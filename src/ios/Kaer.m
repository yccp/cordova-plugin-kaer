#import "Kaer.h"

#ifndef DefineHeader_h
#define DefineHeader_h

#define DEBUGCONNECT_NODELAY NO
#define IsLANXUMDemo 1

#define MAINBLOCK(block) dispatch_async(dispatch_get_main_queue(),block)

//NSUserDefaults 实例化
#define USER_DEFAULT [NSUserDefaults standardUserDefaults]

#endif

#define DefaultIPAddress @"wss://115.28.2.173:7443"//@"115.28.2.173:7443"
#define DefaultUser @"admin"
#define DefaultUserPWD @"www.kaer.cn"

#define DefaultImgDecodeAddress @"https://121.42.36.16:6443/unpackwlt"

#define DefaultIPUserDefaultKey @"IpServUserDefaultKey"
#define DefaultImgDecodeKey @"ImgDecodeUserDefaultKey"

@implementation Kaer
- (void)pluginInitialize
{
    NSLog(@"初始化中...");
    self.dtformatter=[[NSDateFormatter alloc] init];
    self.dtformatter.dateFormat=@"yyyy-MM-dd HH:mm:ss";
    
    NSString *cacheImgDecodedServIp=[USER_DEFAULT valueForKey:DefaultImgDecodeKey];
    if(cacheImgDecodedServIp.length==0){
        self.imgDecodeUrl=DefaultImgDecodeAddress;
    }
    
    NSString *cacheServIp= [USER_DEFAULT valueForKey:DefaultIPUserDefaultKey];
    if(cacheServIp.length==0){
        cacheServIp=DefaultIPAddress;
    }
    if([cacheServIp.uppercaseString hasPrefix:@"WS"]){
        self.idcardServUrl=cacheServIp;
    }else{
        self.idcardServUrl=[NSString stringWithFormat:@"wss://%@",cacheServIp];
    }
    
    [self initKeTool];
}

- (void)readIdCard:(CDVInvokedUrlCommand *)command
{
    // check arguments
    NSString *uuid = [command.arguments objectAtIndex:0];
    if (!uuid)
    {
        [self failWithCallbackID:command.callbackId withMessage:@"参数格式错误"];
        return ;
    }
    
    NSLog(@"uuid: %@", uuid);
    
    self.currentCallbackId = command.callbackId;
    [self readCardintegration:uuid];
}

-(void)readCardintegration:(NSString*)bleIdentify{
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        [self initSSL];
        BOOL status = [self.KEtool connectBt:bleIdentify];
        NSLog(@"状态：%d", status);
        if(status){
            //连接蓝牙
            NSString *cacheServIp= [USER_DEFAULT valueForKey:DefaultIPUserDefaultKey];
            if(cacheServIp.length==0){
                cacheServIp=DefaultIPAddress;
            }
            NSString *cacheImgDecodedServIp=[USER_DEFAULT valueForKey:DefaultImgDecodeKey];
            if(cacheImgDecodedServIp.length==0){
                self.imgDecodeUrl=DefaultImgDecodeAddress;
            }
            
            IDCardItem *carditem= [self.KEtool readCert:0 url:cacheServIp wltImgUrl:self.imgDecodeUrl];
            
            if(carditem.resultCode) {
                [self failWithCallbackID:self.currentCallbackId withMessage:[carditem toString]];
                return;
            }
            
            NSString* image = [carditem.picImgData base64EncodedStringWithOptions:0];
            NSDictionary* data = @ {
                @"name": carditem.partyName,
                @"gender": carditem.gender,
                @"ethnic": carditem.nation,
                @"dateOfBirth": carditem.bornDay,
                @"address": carditem.certAddress,
                @"id": carditem.certNumber,
                @"org": carditem.certOrg,
                @"effDate": carditem.effDate,
                @"expDate": carditem.expDate,
                @"image": [@"data:image/jpeg;base64," stringByAppendingString:image]
            };
            NSLog(@"image: %@", image);
            CDVPluginResult* result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:@{ @"status":@"completed", @"data": data }];
            
            if (result) {
                [[self commandDelegate] sendPluginResult:result callbackId: self.currentCallbackId];
            }
        }
    });
}

- (void)failWithCallbackID:(NSString *)callbackID withMessage:(NSString *)message
{
    CDVPluginResult *commandResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsString:message];
    [self.commandDelegate sendPluginResult:commandResult callbackId:callbackID];
}

#pragma mark -kaerbleAdapter delegate
-(void)SSL_ConnectResult:(BOOL)isconnected{
    if (isconnected) {
        NSLog(@"调用对象回调：ssl连接成功");
    }else{
        NSLog(@"调用对象回调：ssl连接断开");
    }
    
}
-(void)BR_ConnectResult:(BOOL)isconnected{
    if (isconnected) {
        NSLog(@"调用对象回调：ble连接成功");
    }else{
        NSLog(@"调用对象回调：ble连接断开");
    }
}

-(void)KE_ProgressUpdate:(float)progress{
    if(progress<1.0f){
        NSLog(@"当前进度：%f",progress);
        CDVPluginResult* result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:@{ @"status":@"progress", @"progress": [NSNumber numberWithFloat:progress] }];
        
        if (result) {
            [result setKeepCallbackAsBool:YES];
            [[self commandDelegate] sendPluginResult:result callbackId: self.currentCallbackId];
        }
    }
}


-(void)KE_ReadcertResult:(IDCardItem*)resultItem{
    //得到身份证读卡结果
    NSLog(@"获取读卡返回结果");
    MAINBLOCK(^{
        NSString *decstr= [resultItem description];
        NSLog(@"%@", decstr);
    });
}


-(void)KE_ReadcertImageResult:(NSData*)unpackwltData{
    NSLog(@"获取身份证相片解码结果:%@",unpackwltData);
}

-(void)KE_DiscoverBlePeripheral:(BleDevice *)bledevice{
    NSLog(@"发现ble设备：%@",bledevice.BleName);
}

-(void)initKeTool{
    self.KEtool=[KaerAdapter getInstance:self];
}

-(void)initSSL{
    BOOL connstatus=[self.KEtool connectStatusSSL];
    BOOL isnetworktype=[self.KEtool connectedIsNetworkType];
    if(DEBUGCONNECT_NODELAY){
        isnetworktype=YES;
    }
    if(!connstatus && isnetworktype){
        //分体阅读器——检查未连接服务器。连接服务器
        NSString *cacheServIp= [USER_DEFAULT valueForKey:DefaultIPUserDefaultKey];
        if(cacheServIp.length==0){
            cacheServIp=DefaultIPAddress;
        }
        NSString *wssUrl=@"";
        if([cacheServIp.uppercaseString hasPrefix:@"WS"]){
            wssUrl=cacheServIp;
        }else{
            wssUrl=[NSString stringWithFormat:@"wss://%@",cacheServIp];
            
        }
        NSString *uname=DefaultUser;
        NSString *upwd=DefaultUserPWD;
        int connResult= [self.KEtool connectServer:wssUrl withUser:uname andPwd:upwd];
        if(connResult==KE_Success){
            NSLog(@"连接服务器成功");
        }else {
            NSLog(@"连接服务器失败：%d",connResult);
        }
    }
}

@end
