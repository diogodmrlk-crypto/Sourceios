//
//  ContainerAccess.h
//  Dash / InjectorApp
//
//  Reconstructed from binary: External20DASH%202.0.ipa (arm64)
//

#import <Foundation/Foundation.h>
#import "DPContainerGrant.h"

@interface ContainerAccess : NSObject

+ (void)initialize;
+ (NSError *)error:(NSInteger)code message:(NSString *)msg;
+ (BOOL)resolveSymbols;
+ (NSString *)resolveContainerPathByMetadataScan:(NSString *)bundleIdentifier;
+ (DPContainerGrant *)grantViaKernelFallback:(NSString *)bundleIdentifier outPath:(NSString **)outPath error:(NSError **)error;
+ (BOOL)isSupported;
+ (DPContainerGrant *)grantAccessForBundleIdentifier:(NSString *)bundleIdentifier outPath:(NSString **)outPath error:(NSError **)error;
+ (void)releaseHandle:(DPContainerGrant *)grant;
+ (BOOL)withAccessToBundleIdentifier:(NSString *)bundleIdentifier error:(NSError **)error usingBlock:(void (^)(NSString *containerPath))block;

@end
