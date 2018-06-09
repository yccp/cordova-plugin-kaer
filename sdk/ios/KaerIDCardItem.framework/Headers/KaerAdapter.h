//
//  KaerAdapter.h
//卡尔身份证阅读器——IOS sdk
//version：1.36.8
//更新内容：1、增加读DN码功能
//        2、增加timeTag 、nfcSignature字段
//        3、修改DN码
//        4、连接方法增加检测，如果已连接且标识不同，就断开连接，如果已连接且是同一个标识，就直接返回


#import <Foundation/Foundation.h>
#import "KEreader_global.h"
#import "IDCardItem.h"
#import "WaitTimer.h"
#import <CoreBluetooth/CoreBluetooth.h>

#import "KEDelegateProtocol.h"




@interface KaerAdapter : NSObject

@property(assign,nonatomic)id<KaerAdapterDelegate> KEDelegate;


//初始化方法，入参是实现委托的OC对象
+ (instancetype)getInstance:(id)targetDelegateCls ;


///开始搜索蓝牙，如果不调用ble_stopSearch会一直搜索，调用连接蓝牙接口会停止
-(void)ble_startSearch;

///停止蓝牙搜索
-(void)ble_stopSearch;

///扫描周围蓝牙阅读器,定时停止搜索
-(NSMutableArray*)ScanDeviceList:(float)scanTime;//scanTime扫描等待时间，秒

///连接蓝牙
-(BOOL)connectBt:(NSString*)UUID; //uuid 要连接的设备uuid

///蓝牙断开
-(BOOL)disconnectBt;


#pragma mark -集成连接服务器、读身份证、相片解码的接口,可读分体和一体身份证阅读器

/**
 新兼容读身份证接口，此接口兼容设备KT8000 新/旧协议，KT8003协议。
 入参 ：  1、protocolType 一体身份证阅读器的协议类型(此参数与分体阅读器读身份证无关)，
            传值0 默认协议
            传值1 联通集团kt8000一体读身份证协议;
        2、serverUrl 分体身份证阅读器的身份证文本信息解码地址(此参数与一体身份证阅读器读身份证无关)
            如果传nil，则默认使用kaer的云身份证解码服务器
        3、wltServerUrl 身份证照片信息解码服务器
            如果传值nil，则默认使用kaer的云相片解码服务器
        
 note:调用此接口会自动连接身份证解码服务器和相片解码服务器，不需要像以前的版本做连接服务器、断开服务器连接的操作。
 二次开发时的读身份证步骤：
        a. 调用ScanDeviceList方法获取蓝牙列表
        b. 调用connectBt 方法连接到蓝牙设备
        c. 调用此方法读身份证，返回身份证信息，返回IDCardItem 对象的picImgData属性是解码后的照片数据，能直接在UIImageView中显示
        d. 调用disconnectBt方法断开蓝牙连接
 */
-(IDCardItem*)readCert:(int)protocolType
                   url:(NSString*)serverUrl
             wltImgUrl:(NSString*)wltServerUrl;





#pragma mark -Tools
///蓝牙连接状态查询,返回值:YES 已连接  NO 未连接
-(BOOL)connectStatus;

///分体专用-服务器连接状态查询，返回值yes  已连接   no 未连接
-(BOOL)connectStatusSSL;

///当前连接的设备蓝牙名
-(NSString*)connectedBLEName;

///当前连接设备是否是分体设备  返回yes 是分体设备  返回no  一体设备
-(BOOL)connectedIsNetworkType;




#pragma mark -sim卡开卡接口

/**
 *readICCID   读iccid
 *返回值keys:
 *     errCode: NSString 0 成功 其他失败
 *     ICCIDData:NSData iccid数据
 **/
-(NSDictionary *) readICCID;
/**
 *readIMSI   读imsi
 *入参:Byte   0x02 读2G卡   0x03 读3G卡
 *返回值keys:
 *     errCode: NSString 0成功 其他失败
 *     isBlank: NSNumber 0 成卡  1 白卡
 *     IMSI2G:  NSData  2GIMSI  成卡时有此值
 *     IMSI3G:  NSData  3GIMSI  成卡时有此值
 **/
-(NSDictionary*)readIMSI:(Byte)simType;
/**
 *writeIMSI andIMSI2  写imsi
 *入参: IMSI2G(必填) 写IMSI2G  、 IMSI3G(选填) 写IMSI3G ，填nil则只写IMSI2G
 *返回值 int: 0 成功  1失败
 *
 **/
-(int)writeIMSI:(NSString*)IMSI2G andIMSI2:(NSString*)IMSI3G;

/**
 *writeSMSC withNumberIndex  写短信中心号
 *入参: PhoneNumber 短信中心号  、 NumberIndex 短信中心号索引
 *返回值 int: 0成功  1失败
 *
 **/
-(int)writeSMSC:(NSString *)PhoneNumber withNumberIndex:(Byte)NumberIndex;




#pragma mark -其他定制接口

/**
 发送指令关闭蓝牙
 
 @return 设备响应结果
 */
-(NSData*)command_BLE_turnoff;

/**
 发送指令设备进入休眠
 
 @return 设备响应结果
 */
-(NSData*)command_SleepMode;


/**
 获取设备版本号
 */
-(NSString *)getDeviceVersion;

/**
 卡片上电
 
 @return 0 函数执行成功 ， 非0 函数执行失败
 */
-(int)cardPowerOn;



/**
 卡片下电
 
 @return 0 函数执行成功 ， 非0 函数执行失败
 */
-(int)cardPowerOff;



/**
 发送APDU指令
 
 @param sendData 要发送的指令
 @param resultData 接收到的指令
 @return 0 函数执行成功 ， 非0 函数执行失败
 */
-(int)sendApduCommand:(NSData*)sendData out:(NSData**)resultData;



/**
 一体身份证阅读器专用-获取SAMID
 
 @return samid结果
 */
-(NSData *)getSAMNo;


/**
 读身份证后获取S1(摘要)
 
 @return S1(摘要)结果
 */
-(NSData*)getS1;

/**
 读身份证后获取H1
 */
-(NSData*)getH1;

/**
 读身份证后获取C1
 note:获取C1之前必须先获取S1，调用顺序 getS1->getH1->getC1
 */
-(NSData*)getC1;



+(NSString*)getSDKVersion;


#pragma mark -以下接口是兼容旧版本(低于v1.32)保留的旧sdk接口。新读卡接口已更改，第一次对接或要兼容联通KT8000身份证阅读器协议的读卡请忽略以下接口

///连接服务器   返回值0 成功，其他失败
-(int)connectServer:(NSString*)siteUrl withUser:(NSString *)uname andPwd:(NSString*)upwd;

///读身份证
-(IDCardItem*)readCert;


///分体专用-断开服务器
-(BOOL)disconnectSSL;



/**
 *  获取最后一次读卡数据的照片的wlt解码后数据
 *
 *  @return 返回类型NSDictionary<br>
 *  keys:<br>
 *  errCode 0为解码成功 -1解码失败<br>
 *  errDesc 在解码失败时有效，失败错误信息描述<br>
 *  DecPicData NSData型，解码成功时该key有效,解码后的相片信息
 */
-(NSDictionary*)GetDecodedPicData;

/**
 *  获取最后一次读卡数据的照片的wlt解码后数据 ,可传入服务器地址
 *
 *  @param ServUrl 自建的可用相片解码服务器外网地址
 *
 *  @return 返回类型NSDictionary<br>
 *  keys:<br>
 *  errCode 0为解码成功 -1解码失败<br>
 *  errDesc 在解码失败时有效，失败错误信息描述<br>
 *  DecPicData NSData型，解码成功时该key有效,解码后的相片信息
 */
-(NSDictionary*)getDecodedPicDataOnServerUrl:(NSString*)ServUrl;



#pragma mark -只调用一次，接口内部完成连接蓝牙、服务器，读身份证，解照片，断开蓝牙服务器操作，返回身份证结果
/**
 * 浙江移动测试接口。只调用一次，接口内部完成连接蓝牙、连接服务器、读身份证、解照片、断开蓝牙、断开服务器操作，返回身份证结果
 */
-(IDCardItem*)readCertIntegrationInterface:(int) protype
                               BLEIdentify:(NSString*) bleuuid
                              IDCardServer:(NSString*) idcardserveraddr
                              PicImgServer:(NSString*) picimgdecodeserver;


@end
