//
//  ContainerAccess.m
//  Dash / InjectorApp
//
//  Reconstructed container scanner and fallback writer
//

#import "ContainerAccess.h"
#import "KernelAccess.h"

@implementation ContainerAccess

+ (void)initialize {
}

+ (NSError *)error:(NSInteger)code message:(NSString *)msg {
    return [NSError errorWithDomain:@"com.dash.patcher.containeraccess"
                               code:code
                           userInfo:@{NSLocalizedDescriptionKey: msg}];
}

+ (BOOL)resolveSymbols {
    return YES;
}

+ (NSString *)resolveContainerPathByMetadataScan:(NSString *)bundleIdentifier {
    NSArray *baseDirs = @[
        @"/private/var/mobile/Containers/Data/Application",
        @"/var/mobile/Containers/Data/Application"
    ];
    
    NSFileManager *fm = [NSFileManager defaultManager];
    for (NSString *base in baseDirs) {
        NSError *err = nil;
        NSArray *subdirs = [fm contentsOfDirectoryAtPath:base error:&err];
        if (err || !subdirs) continue;
        
        for (NSString *sub in subdirs) {
            NSString *metaPlist = [base stringByAppendingPathComponent:[NSString stringWithFormat:@"%@/.com.apple.mobile_container_manager.metadata.plist", sub]];
            if ([fm fileExistsAtPath:metaPlist]) {
                NSDictionary *plist = [NSDictionary dictionaryWithContentsOfFile:metaPlist];
                if ([plist[@"MCMMetadataIdentifier"] isEqualToString:bundleIdentifier] ||
                    [plist[@"identifier"] isEqualToString:bundleIdentifier]) {
                    return [base stringByAppendingPathComponent:sub];
                }
            }
        }
    }
    return nil;
}

+ (DPContainerGrant *)grantViaKernelFallback:(NSString *)bundleIdentifier outPath:(NSString **)outPath error:(NSError **)error {
    if ([KernelAccess isReady] && [KernelAccess activePath] == KernelAccessPathKernel) {
        NSString *container = [self resolveContainerPathByMetadataScan:bundleIdentifier];
        if (container) {
            // Probe write permission
            NSString *probe = [container stringByAppendingPathComponent:[NSString stringWithFormat:@".dash-probe-%@", [[NSUUID UUID] UUIDString]]];
            if ([@"ok" writeToFile:probe atomically:YES encoding:NSUTF8StringEncoding error:nil]) {
                [[NSFileManager defaultManager] removeItemAtPath:probe error:nil];
                if (outPath) *outPath = container;
                
                DPContainerGrant *grant = [[DPContainerGrant alloc] init];
                grant.path = container;
                NSLog(@"[ContainerAccess] kernel/metadata fallback OK for %@ path=%@", bundleIdentifier, container);
                return grant;
            }
        }
    }
    if (error) *error = [self error:-1 message:[NSString stringWithFormat:@"Nao foi possivel obter container de %@", bundleIdentifier]];
    return nil;
}

+ (BOOL)isSupported {
    return [KernelAccess isReady];
}

+ (DPContainerGrant *)grantAccessForBundleIdentifier:(NSString *)bundleIdentifier outPath:(NSString **)outPath error:(NSError **)error {
    return [self grantViaKernelFallback:bundleIdentifier outPath:outPath error:error];
}

+ (void)releaseHandle:(DPContainerGrant *)grant {
    grant.path = nil;
    grant.object = nil;
}

+ (BOOL)withAccessToBundleIdentifier:(NSString *)bundleIdentifier error:(NSError **)error usingBlock:(void (^)(NSString *containerPath))block {
    NSString *path = nil;
    DPContainerGrant *grant = [self grantAccessForBundleIdentifier:bundleIdentifier outPath:&path error:error];
    if (!grant || !path) return NO;
    
    if (block) {
        block(path);
    }
    [self releaseHandle:grant];
    return YES;
}

@end
