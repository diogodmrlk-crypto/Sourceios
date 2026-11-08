//
//  FWEncryptorAES.h
//  Dash / InjectorApp
//
//  Reconstructed from binary: External20DASH%202.0.ipa (arm64)
//

#import <Foundation/Foundation.h>

@interface FWEncryptorAES : NSObject

+ (NSData *)encrypt:(NSData *)data Key:(NSString *)key IV:(NSString *)iv;
+ (NSData *)decrypt:(NSData *)data Key:(NSString *)key IV:(NSString *)iv;
+ (NSString *)encryptToBase64:(NSString *)string Key:(NSString *)key IV:(NSString *)iv;
+ (NSString *)decryptFromBase64:(NSString *)base64String Key:(NSString *)key IV:(NSString *)iv;
+ (NSData *)getKey:(NSString *)keyStr;
+ (NSData *)getIV:(NSString *)ivStr;
+ (NSString *)convertHexStringFromData:(NSData *)data;

@end
