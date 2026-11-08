//
//  KernelAccess.h
//  Dash / InjectorApp
//
//  Reconstructed from binary: External20DASH%202.0.ipa (arm64)
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, KernelAccessActivePath) {
    KernelAccessPathNone     = 0,
    KernelAccessPathAirLift  = 1,
    KernelAccessPathKernel   = 2,
    KernelAccessPathMHA      = 3
};

@interface KernelAccess : NSObject

+ (BOOL)isReady;
+ (NSInteger)activePath;
+ (NSString *)statusMessage;
+ (NSString *)systemVersion;
+ (BOOL)version:(NSString *)version atLeast:(NSString *)minVersion lessThan:(NSString *)maxVersion;
+ (BOOL)supportsKernelPath;
+ (BOOL)supportsMHAPath;
+ (BOOL)supportsAirLiftPath;
+ (void)activateMHAPath;
+ (BOOL)startAirLiftPath:(NSError **)error;
+ (BOOL)startKernelPath:(NSError **)error;
+ (BOOL)startRecommendedPath:(NSError **)error;
+ (void)resetState;

@end
