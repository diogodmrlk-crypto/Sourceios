//
//  RootViewController.h
//  Dash / InjectorApp
//
//  Reconstructed from binary: External20DASH%202.0.ipa (arm64)
//

#import <UIKit/UIKit.h>

@interface RootViewController : UIViewController <UITableViewDataSource, UITableViewDelegate>

// UI Lifecycle
- (void)viewDidLoad;
- (void)configureButtonLayout:(id)sender;
- (void)toggleLightTheme;
- (void)traitCollectionDidChange:(UITraitCollection *)previousTraitCollection;
- (void)viewDidLayoutSubviews;

// Localization & Device Specs
- (NSString *)L:(NSString *)key;
- (NSString *)hardwareModel;
- (NSString *)commercialDeviceName;
- (void)buildDeviceRowsInCard:(UIView *)card width:(CGFloat)width;
- (void)refreshDeviceValues;
- (void)applyLanguage;
- (void)openSettingsSheet;

// Authentication
- (void)startAPIAuthentication;
- (NSString *)stringFromAPIValue:(id)val fallback:(NSString *)fb;
- (void)refreshAPIAccountValues;

// Tabs Navigation
- (UIView *)visibleMainPage;
- (void)animateToMainPage:(UIView *)targetPage;
- (void)tabExploitPressed;
- (void)tabNoExploitPressed;
- (void)tabMiscPressed;
- (void)tabAccountPressed;
- (void)setTabActive:(UIButton *)activeBtn inactive:(UIButton *)inactiveBtn;

// Injector & Game UI Switchers
- (NSAttributedString *)injectorAttributedTitle:(NSString *)title subtitle:(NSString *)sub selected:(BOOL)sel;
- (void)styleOptionChip:(UIButton *)chip selected:(BOOL)sel;
- (void)applyInjectorTabAppearance;
- (void)applyGameTabAppearance;
- (void)selectInjector1;
- (void)selectInjector2;
- (void)selectExpFF;
- (void)selectExpFFMax;
- (void)selectNoExpFF;
- (void)selectNoExpFFMax;
- (void)animateTable:(UITableView *)tv goingToMax:(BOOL)isMax reload:(BOOL)reload;

// Exploit Controller
- (NSArray *)visibleFunctionRows;
- (NSString *)currentBundleIDForTable:(UITableView *)tv;
- (void)toggleExploitStart;

// Payload Paths & URLs
- (NSString *)backupRoot;
- (BOOL)isTexturePayloadRow:(NSInteger)row;
- (NSInteger)textureIndexForPayloadRow:(NSInteger)row;
- (NSString *)textureFolderNameAtIndex:(NSInteger)index;
- (NSString *)textureCDNPath:(NSString *)folder file:(NSString *)file;
- (NSURL *)texturePreviewURLAtIndex:(NSInteger)index;
- (NSURL *)texturePayloadURLAtIndex:(NSInteger)index;
- (NSString *)backupPathForBundleID:(NSString *)bundleID row:(NSInteger)row;
- (BOOL)isInj2AssetindexerRow:(NSInteger)row;
- (NSString *)targetRelativePathForRow:(NSInteger)row bundleID:(NSString *)bundleID;
- (NSData *)embeddedPayloadDataForRow:(NSInteger)row;
- (NSURL *)payloadURLForRow:(NSInteger)row bundleID:(NSString *)bundleID;
- (NSString *)payloadCachePathForRow:(NSInteger)row bundleID:(NSString *)bundleID;

// Network & Injection Engine
- (void)loadPayloadFromURLString:(NSString *)urlStr completion:(void (^)(NSData *data, NSError *error))completion;
- (BOOL)injectData:(NSData *)data forBundleID:(NSString *)bundleID relativePath:(NSString *)relPath error:(NSError **)error;
- (BOOL)deleteDataForBundleID:(NSString *)bundleID relativePath:(NSString *)relPath error:(NSError **)error;
- (void)loadPayloadForRow:(NSInteger)row bundleID:(NSString *)bundleID completion:(void (^)(NSData *data, NSError *error))completion;
- (BOOL)_writePayload:(NSData *)data toBundleID:(NSString *)bundleID relativePath:(NSString *)relPath error:(NSError **)error;
- (BOOL)ensureKernelReady:(NSError **)error;
- (BOOL)injectPayload:(NSData *)data forBundleID:(NSString *)bundleID row:(NSInteger)row error:(NSError **)error;
- (BOOL)restorePayloadForBundleID:(NSString *)bundleID row:(NSInteger)row error:(NSError **)error;

// User Prompts & Toggles
- (void)showAlert:(NSString *)title message:(NSString *)msg;
- (void)askAimAssistBoxThenActivate:(NSInteger)row injector1:(BOOL)inj1;
- (void)deactivateAimAssist:(NSInteger)row;
- (void)miscSwitchChanged:(UISwitch *)sender;
- (void)askAntennaThenInjectRow:(NSInteger)row sender:(UISwitch *)sender;
- (void)performInj2InjectRow:(NSInteger)row sender:(UISwitch *)sender;
- (void)switchChanged:(UISwitch *)sender;
- (void)inj1SwitchChanged:(UISwitch *)sender;
- (void)textureSwitchChanged:(UISwitch *)sender;
- (void)resetToggles;

// Textures & Visual Helpers
- (UIImage *)emojiImage:(NSString *)emoji size:(CGSize)size;
- (UIImage *)textureThumbPlaceholder;
- (UIImage *)resizedTextureThumb:(UIImage *)img;
- (void)bindTextureThumbAtIndex:(NSInteger)index toImageView:(UIImageView *)iv;

@end
