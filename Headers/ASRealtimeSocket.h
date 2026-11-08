//
//  ASRealtimeSocket.h
//  Dash / InjectorApp
//
//  Reconstructed from binary: External20DASH%202.0.ipa (arm64)
//

#import <Foundation/Foundation.h>

extern NSString * const ASRealtimeStatusConnecting;
extern NSString * const ASRealtimeStatusConnected;
extern NSString * const ASRealtimeStatusDisconnected;

@interface ASRealtimeSocket : NSObject

@property (nonatomic, assign) BOOL isConnected;

- (instancetype)initWithBaseURL:(NSString *)url headers:(NSDictionary *)headers query:(NSDictionary *)query;
- (void)connect;
- (void)disconnect;
- (void)emit:(NSString *)event items:(NSArray *)items;
- (void)on:(NSString *)event handler:(void (^)(id data))handler;
- (void)onStatusChange:(void (^)(NSString *status, NSString *reason))handler;

@end
