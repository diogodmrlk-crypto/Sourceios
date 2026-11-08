//
//  NetTool.h
//  Dash / InjectorApp
//
//  Reconstructed from binary: External20DASH%202.0.ipa (arm64)
//

#import <Foundation/Foundation.h>

@interface NetTool : NSObject

+ (NSURLSessionDataTask *)Post_AppendURL:(NSString *)url
                            myparameters:(NSDictionary *)params
                               mysuccess:(void (^)(id response))success
                               myfailure:(void (^)(NSError *error))failure;

+ (SecKeyRef)getPublicKeyFromString:(NSString *)keyString;
+ (BOOL)verifySignature:(NSData *)signature withData:(NSData *)data usingPublicKeyString:(NSString *)pubKey;

@end
