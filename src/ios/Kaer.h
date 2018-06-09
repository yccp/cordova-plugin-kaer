#import <Cordova/CDVPlugin.h>
#import <KaerIDCardItem/KaerIDCardItem.h>

@interface Kaer : CDVPlugin<KaerAdapterDelegate>

@property (nonatomic, strong) NSString *imgDecodeUrl;
@property (nonatomic, strong) NSString *idcardServUrl;
@property (nonatomic, strong) NSString *currentCallbackId;
@property (nonatomic, strong) KaerAdapter *KEtool;
@property (nonatomic, strong) NSDateFormatter* dtformatter;

- (void)readIdCard:(CDVInvokedUrlCommand *)command;

@end
