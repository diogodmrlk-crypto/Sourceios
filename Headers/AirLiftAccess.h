//
//  AirLiftAccess.h
//  Dash / InjectorApp
//
//  Reconstructed from binary: External20DASH%202.0.ipa (arm64)
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, AirLiftStatus) {
    AirLiftStatusIdle       = 0,
    AirLiftStatusConnecting = 1,
    AirLiftStatusReady      = 2,
    AirLiftStatusError      = 3
};

@interface AirLiftAccess : NSObject

+ (NSInteger)status;
+ (BOOL)isReady;
+ (NSString *)statusMessage;
+ (BOOL)_loadAFCSymbols;
+ (void *)_openLocalAFCConnection;
+ (NSString *)_resolveContainerUUID:(NSString *)bundleID conn:(void *)afcConn;
+ (void)_makedirs:(NSString *)path conn:(void *)afcConn;
+ (BOOL)prepare:(NSError **)error;
+ (BOOL)writeData:(NSData *)data toBundleID:(NSString *)bundleID relativePath:(NSString *)relPath error:(NSError **)error;
+ (NSData *)readDataFromBundleID:(NSString *)bundleID relativePath:(NSString *)relPath error:(NSError **)error;

@end
