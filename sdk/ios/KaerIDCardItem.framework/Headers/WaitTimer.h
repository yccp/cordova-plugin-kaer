//
//  WaitTimer.h
//  SharedTypeIDCardReader
//
//  Created by kaer on 15-10-16.
//  Copyright (c) 2015年 xc. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface WaitTimer : NSObject{
    BOOL timerFinished;
    NSTimer *delayForProgram;
}

+ (WaitTimer *)getInstance ;

+ (WaitTimer *)getMainInstance;

///阻塞等待$times 秒
-(void) waitTimeForSeconds:(NSTimeInterval) times;

///停止等待
-(void)stopWait;
@end
