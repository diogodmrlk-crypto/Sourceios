//
//  Language.h
//  Dash / InjectorApp
//
//  Reconstructed from binary: External20DASH%202.0.ipa (arm64)
//

#import <Foundation/Foundation.h>

@interface Language : NSObject

@property (nonatomic, strong) NSDictionary *languageStrings;

+ (instancetype)sharedInstance;
- (void)loadLanguageData;
- (NSString *)t:(NSString *)key language:(NSString *)lang;

@end
