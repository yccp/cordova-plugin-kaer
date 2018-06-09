//
//  kt8003reader_global.h
//  SharedTypeIDCardReader
//
//  Copyright (c) 2015年 xc. All rights reserved.
//

#ifndef SharedTypeIDCardReader_KEreader_global_h
#define SharedTypeIDCardReader_KE8003reader_global_h


enum KEErrorCode{
    KE_Success = 0,
    KE_PortError = 0x01,//端口打开失败 /端口尚未打开 端口尚未打开 /端口号不合法 端口号不合法
    KE_Timeout = 0x02,//接收超时，在规定的间内未到长度数据
    KE_TranslateError = 0x03,//数据传输错误
    KE_COMUseless = 0x05,//SAM_A 串口不可用，只 串口不可用，只 有 SDT_GetCOMBaud SDT_GetCOMBaud 函数 返,not useful
    
    KE_OperationSuccess = 0x90,//操作成功
    KE_NotContent= 0x91,//居民身份证 中无 此项 内容
    KE_FindCardSuccess=  0x9F,//寻找 居民 身份证 成功
    KE_OpenFileError = 0x09,//打开文件失败
    KE_CheckSumError = 0x10,//接收业务终端数据的校验和错
    KE_DataLengthError = 0x11,//接收业务终端数据的长度错
    KE_CommandError = 0x21,//接收业务终端的命令错误
    KE_UnauthorizedOperation = 0x23,//越权操作
    KE_UnrecorgnizeError = 0x24,//无法识别的错误
    KE_FindCardError = 0x80,//寻找 居民身份证 失败
    KE_SelectCardError = 0x81,//选取 居民身 份证 失败
    KE_SAMVAuthFailed = 0x31,//居民身份证 认证 SAM_A 失败
    KE_CardAuthFailed = 0x32,//SAM_A 认证 居民身份证 居民身份证 失败
    KE_VerifyInfoFailed = 0x33,//信息验证失败
    KE_FingerprintError = 0x37,//指纹信息验证错误
    KE_InfoLenError = 0x3F,//信息长度错误
    KE_WrongCard = 0x40,//错误的卡
    KE_ReadCardFailed = 0x41,//读居民身份证 操作失败
    KE_RandonNumFailed = 0x47,//取随机数失败
    KE_SAMVSelfCheckError = 0x60,//SAM_A 自检失败，不能接收命令
    KE_NoAuthSAMV = 0x66  ,//SAM_A 没经过授权 ，无法使用
    
    KE_WltUnpackError = 6,
    KE_ConnectServerError = 7,
    KE_ParamError = 8,
    KE_RemoteSAMVDeviceError = 0x67,//远程samv设备错误
    KE_ClaimRemoteSAMVError = 0x69,//远程获取samv失败
    KE_SIMCardNotInsert = 0x71,//sim卡未插入
    KE_SIMCardUnrecorgnize = 0x72,//sim卡插入但不识别
    KE_SIMCardNotRead = 0x73,//sim卡插入但还未成功读取
    KE_SIMCardNotInit = 0x74,//sim卡插入但还未初始化
    
    KE_UnknownError = 0xA0,
    KE_AlreadyOpened = 0xA1,
    KE_DecryptError = 0xA2,
    KE_DeviceNotSupport = 0xA3,//不支持的设备
    
    KE_PreReadError = 0xB1,
    KE_LoginCMSError = 0xB2,//登录失败
    KE_SemaphoreError = 0xB3 ,//操作未完成
    KE_StateError = 0xB4,//状态错误
    
    KE_NeedLogin = 0xB5,//未登录
    KE_PwdError = 0xB6,//密码错误
    KE_UnKnownUser = 0xB7,//用户名不存在
    KE_LimitedUser = 0xB8,//用户被限制登陆
    KE_NoAvailableDecoder = 0xB9,//无可用解码集群
    
    
    KE_BTH_Unconnceted=0xBA, //蓝牙未连接
    KE_BTH_Broken=0xBB, //蓝牙连接异常断开
    KE_BTH_BLOCK=0xBC, //可能由于距离过远引起的阻塞
    KE_READ_UNFINISHED=0xD0, //读取未完成
    KE_NFC_TagLost=0xD1, //读取过程身份证对象丢失，大部分因为移动导致
    KE_NFC_TECH_NotDiscover=0xD2,//不是Tech格式的卡
    KE_NFC_NFCB_NotDiscover=0xD3,//不是NfcB模式卡
    KE_NFC_NULL_TAG=0xD5, //获取到tag是空指针（没出现过）
    KE_OTG_Unconnected=0xD9, //usb口没有插入设备，或插入不识别
    KE_OTG_PermissionDenied=0xDA, //OTG设备没有获取到许可
    KE_OTG_Unmatched=0xDB ,//设备vid pid不满足要求
    KE_FOREIGNER_NOTSUPPORT=0x1001,//当前协议不支持外国人永居证
    
};

#endif
