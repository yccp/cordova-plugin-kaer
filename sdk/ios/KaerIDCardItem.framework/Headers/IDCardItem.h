//
//  IDCardItem.h
//  SharedTypeIDCardReader
//
//  Created by kaer on 15-10-15.
//  Copyright (c) 2015年 xc. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface IDCardItem : NSObject

@property int resultCode;
@property (nonatomic,retain) NSString *resultErrDesc;

@property int cardType;//0国内居民身份证  1外国人永久居留身份证
@property (nonatomic,retain) NSString *partyName;//姓名，如果证件是外国人永久居留身份证，此字段是中文姓名
@property (nonatomic,retain) NSString *englishName;//外国人永久居留身份证英文姓名字段
@property (nonatomic,retain) NSString *gender;//性别
@property (nonatomic,retain) NSString *nation;//民族，外国人永久居留身份证的国籍或所在地区代码字段

@property (nonatomic,retain) NSString *bornDay;//出生日期
@property (nonatomic,retain) NSString *certAddress;//家庭住址，外国人永居证此字段无效
@property (nonatomic ,retain) NSString *certNumber;//证件号码 身份证18位，外国人永居证 15位
@property (nonatomic,retain) NSString *certOrg;//签发机关

@property (nonatomic,retain) NSString *effDate;//证件签发日期
@property (nonatomic,retain) NSString *expDate;//证件终止日期

@property (nonatomic,retain) NSData *picImgData;//身份证照片wlt加密数据
@property (nonatomic,retain) NSData *fingerInfoData;//指纹数据

@property (nonatomic,retain) NSString *respUUID;
@property (nonatomic,retain) NSString *dn;
@property (nonatomic,retain) NSString *timeTag;
@property (nonatomic,retain) NSString *nfcSignature;


//@property (nonatomic,retain) NSData *SAMIDData;
//@property (nonatomic,retain) NSData *Zhaiyao;

-(IDCardItem*)initIDCardItem:(NSData*)picInfoData;
-(void)setErrDesc:(int)errCode;


-(NSString*)toString;

@end

@interface BleDevice : NSObject

@property (nonatomic,retain) NSString *BleIdentify;
@property (nonatomic,retain) NSString *BleName;

@end


