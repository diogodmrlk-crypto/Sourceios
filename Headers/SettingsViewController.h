//
//  SettingsViewController.h
//  Dash / InjectorApp
//
//  Reconstructed from binary: External20DASH%202.0.ipa (arm64)
//

#import <UIKit/UIKit.h>

@interface SettingsViewController : UIViewController

@property (nonatomic, copy) NSString *languageCode;
@property (nonatomic, copy) NSString *versionText;
@property (nonatomic, copy) void (^languageChangedHandler)(NSString *newLang);

- (instancetype)initWithLanguage:(NSString *)lang version:(NSString *)ver languageChanged:(void (^)(NSString *))handler;
- (NSString *)hardwareModel;
- (NSString *)L:(NSString *)key;
- (void)viewDidLoad;
- (void)applyLanguage;
- (void)doneTapped;
- (void)languageChanged:(UISegmentedControl *)sender;
- (void)creditsTapped;

@end
