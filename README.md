# 卡尔身份证读卡器 cordova 插件

## 安装

```
cordova plugin add cordova-plugin-kaer --save
```
或
```
ionic cordova plugin add cordova-plugin-kaer
```

## 身份证读取

```js
window.Kaer.readIdCard(uuid, x => {
  console.log(x);
  if(x.status === 'progress') {
    console.log(x.progress);
    return;
  }

  if(x.status === 'completed') {
    console.log(x.data);
    return;
  }
}, e => {
  console.error(e);
});

```