//
//  RootViewController.m
//  Dash / InjectorApp
//
//  Reconstructed main controller, injection UI, and cheat payload manager
//

#import "RootViewController.h"
#import "KernelAccess.h"
#import "ContainerAccess.h"
#import "AirLiftAccess.h"
#import "SettingsViewController.h"

static NSString * const kFreeFireBundleID    = @"com.dts.freefireth";
static NSString * const kFreeFireMaxBundleID = @"com.dts.freefiremax";

@interface RootViewController ()
@property (nonatomic, copy) NSString *selectedBundleID;
@property (nonatomic, assign) NSInteger activeInjector; // 1 = Injetor 1 (Apenas HS), 2 = Injetor 2 (HS + Antena)
@property (nonatomic, strong) NSMutableDictionary *activeToggles;
@end

@implementation RootViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.selectedBundleID = kFreeFireBundleID;
    self.activeInjector = 1;
    self.activeToggles = [NSMutableDictionary dictionary];
    
    [self applyLanguage];
    [self startAPIAuthentication];
}

- (void)applyLanguage {
    self.title = @"Dash iOS";
}

- (void)startAPIAuthentication {
    // Authenticates UDID and License token with backend
    NSLog(@"[Dash] Authenticating device...");
}

- (void)selectInjector1 {
    self.activeInjector = 1;
    [self applyInjectorTabAppearance];
}

- (void)selectInjector2 {
    self.activeInjector = 2;
    [self applyInjectorTabAppearance];
}

- (void)selectExpFF {
    self.selectedBundleID = kFreeFireBundleID;
}

- (void)selectExpFFMax {
    self.selectedBundleID = kFreeFireMaxBundleID;
}

- (void)toggleExploitStart {
    if ([KernelAccess isReady]) {
        [KernelAccess resetState];
    } else {
        NSError *err = nil;
        if (![KernelAccess startRecommendedPath:&err]) {
            [self showAlert:@"Exploit falhou" message:[KernelAccess statusMessage]];
        }
    }
}

// Payload target resolution
- (NSString *)targetRelativePathForRow:(NSInteger)row bundleID:(NSString *)bundleID {
    if (row == 0) {
        return @"Library/Preferences/com.dts.freefireth.plist";
    } else if (row == 1) {
        return @"Library/Caches/cache_res.GkLlYqzsX4AtTdE55sDMRh9sJOI~3D";
    } else if (row == 2) {
        return @"Library/Caches/assetindexer.U6Zffc4YIR3DslNj3cXvYGAqz58~3D";
    } else if (row == 3) {
        return @"Documents/Assembly-CSharp-patch.bytes";
    } else if (row == 4) {
        return @"Library/Caches/shaders.P0K3UG2TfecMBhWMMV~2Fu8ReudIk~3D";
    }
    return [NSString stringWithFormat:@"Library/Caches/optionalab_avatar_66.DfUs7MzeaoXWJ4jWN8zRBmYoY7Q~3D"];
}

- (NSURL *)payloadURLForRow:(NSInteger)row bundleID:(NSString *)bundleID {
    switch (row) {
        case 0: return [NSURL URLWithString:@"https://cdn.dashproxy.xyz/ipa/FPS/com.dts.freefireth.plist"];
        case 1: return [NSURL URLWithString:@"https://cdn.dashproxy.xyz/ipa/injetor1/hs-alto/cache_res.GkLlYqzsX4AtTdE55sDMRh9sJOI~3D"];
        case 2: return [NSURL URLWithString:@"https://cdn.dashproxy.xyz/ipa/HS%20E%20ANTENA/assetindexer.U6Zffc4YIR3DslNj3cXvYGAqz58~3D"];
        case 3: return [NSURL URLWithString:@"https://cdn.dashproxy.xyz/ipa/assembly/Aimbot/Assembly-CSharp-patch.bytes"];
        case 4: return [NSURL URLWithString:@"https://cdn.dashproxy.xyz/ipa/Hologram%20red/shaders.P0K3UG2TfecMBhWMMV~2Fu8ReudIk~3D"];
        default: return [NSURL URLWithString:@"https://cdn.dashproxy.xyz/ipa/Antena/magic/assetindexer.U6Zffc4YIR3DslNj3cXvYGAqz58~3D"];
    }
}

- (NSString *)backupPathForBundleID:(NSString *)bundleID row:(NSInteger)row {
    NSString *docs = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) firstObject];
    NSString *backupDir = [docs stringByAppendingPathComponent:[NSString stringWithFormat:@"InjectorBackups/%@", bundleID]];
    [[NSFileManager defaultManager] createDirectoryAtPath:backupDir withIntermediateDirectories:YES attributes:nil error:nil];
    return [backupDir stringByAppendingPathComponent:[NSString stringWithFormat:@"backup_row_%ld.bak", (long)row]];
}

- (BOOL)_writePayload:(NSData *)data toBundleID:(NSString *)bundleID relativePath:(NSString *)relPath error:(NSError **)error {
    if ([KernelAccess activePath] == KernelAccessPathAirLift) {
        return [AirLiftAccess writeData:data toBundleID:bundleID relativePath:relPath error:error];
    } else {
        return [ContainerAccess withAccessToBundleIdentifier:bundleID error:error usingBlock:^(NSString *containerPath) {
            NSString *dest = [containerPath stringByAppendingPathComponent:relPath];
            [[NSFileManager defaultManager] createDirectoryAtPath:[dest stringByDeletingLastPathComponent] withIntermediateDirectories:YES attributes:nil error:nil];
            [data writeToFile:dest atomically:YES];
        }];
    }
}

- (void)loadPayloadForRow:(NSInteger)row bundleID:(NSString *)bundleID completion:(void (^)(NSData *, NSError *))completion {
    NSURL *url = [self payloadURLForRow:row bundleID:bundleID];
    [[[NSURLSession sharedSession] dataTaskWithURL:url completionHandler:^(NSData *data, NSURLResponse *res, NSError *err) {
        dispatch_async(dispatch_get_main_queue(), ^{
            completion(data, err);
        });
    }] resume];
}

- (BOOL)injectPayload:(NSData *)data forBundleID:(NSString *)bundleID row:(NSInteger)row error:(NSError **)error {
    NSString *relPath = [self targetRelativePathForRow:row bundleID:bundleID];
    
    // Backup original if exists
    NSString *backupPath = [self backupPathForBundleID:bundleID row:row];
    if (![[NSFileManager defaultManager] fileExistsAtPath:backupPath]) {
        [ContainerAccess withAccessToBundleIdentifier:bundleID error:nil usingBlock:^(NSString *containerPath) {
            NSString *orig = [containerPath stringByAppendingPathComponent:relPath];
            [[NSFileManager defaultManager] copyItemAtPath:orig toPath:backupPath error:nil];
        }];
    }
    
    return [self _writePayload:data toBundleID:bundleID relativePath:relPath error:error];
}

- (BOOL)restorePayloadForBundleID:(NSString *)bundleID row:(NSInteger)row error:(NSError **)error {
    NSString *relPath = [self targetRelativePathForRow:row bundleID:bundleID];
    NSString *backupPath = [self backupPathForBundleID:bundleID row:row];
    
    if ([[NSFileManager defaultManager] fileExistsAtPath:backupPath]) {
        NSData *orig = [NSData dataWithContentsOfFile:backupPath];
        if (orig) {
            BOOL ok = [self _writePayload:orig toBundleID:bundleID relativePath:relPath error:error];
            if (ok) {
                [[NSFileManager defaultManager] removeItemAtPath:backupPath error:nil];
            }
            return ok;
        }
    }
    return NO;
}

- (void)showAlert:(NSString *)title message:(NSString *)msg {
    UIAlertController *ac = [UIAlertController alertControllerWithTitle:title message:msg preferredStyle:UIAlertControllerStyleAlert];
    [ac addAction:[UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:nil]];
    [self presentViewController:ac animated:YES completion:nil];
}

@end
