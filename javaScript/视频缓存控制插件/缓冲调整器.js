// ==UserScript==
// @name         视频缓存
// @namespace    http://tampermonkey.net/
// @version      0.1
// @description  try to take over the world!
// @author       You
// @match    https://*.qq.com/*
// @exclude  https://user.qzone.qq.com/*
// @match    https://www.weiyun.com/video_*
// @match    https://v.youku.com/v_play/*
// @match    https://v.youku.com/v_show/id_*
// @match    https://vku.youku.com/live/*
// @match    https://video.tudou.com/v/*
// @match    https://www.iqiyi.com/*
// @match    https://live.bilibili.com/*
// @match    https://www.bilibili.com/*
// @match    https://www.ixigua.com/*
// @match    https://www.toutiao.com/video/*
// @match    https://www.acfun.cn/*
// @match    https://live.acfun.cn/live/*
// @match    http://v.pptv.com/show/*
// @match    https://v.pptv.com/show/*
// @match    https://www.miguvideo.com/*
// @match    https://tv.sohu.com/*
// @match    https://film.sohu.com/album/*
// @match    https://www.mgtv.com/*
// @match    https://pan.baidu.com/*
// @match    https://yun.baidu.com/*
// @match    https://*.163.com/*
// @match    https://*.icourse163.org/*
// @match    http://video.sina.*/*
// @match    https://video.sina.*/*
// @match    http://k.sina.*/*
// @match    https://k.sina.*/*
// @match    https://weibo.com/*
// @match    https://*.weibo.com/*
// @match    https://pan.baidu.com/*
// @match    https://yun.baidu.com/*
// @match    http://v.ifeng.com/*
// @match    https://v.ifeng.com/*
// @match    http://news.mtime.com/*
// @match    http://video.mtime.com/*
// @match    https://www.youtube.com/*
// @match    https://www.ted.com/talks/*
// @match    https://www.twitch.tv/*

// @match    https://www.yy.com/*
// @match    https://www.huya.com/*
// @match    https://v.douyu.com/*
// @match    https://www.douyu.com/*
// @match    https://live.douyin.com/*
// @match    https://www.douyin.com/*

// @match    https://www.longzhu.com/*
// @match    https://www.zhanqi.tv/*
// @run-at     document-start
// @require    https://cdn.staticfile.org/vue/2.6.11/vue.min.js
// @require    https://cdn.staticfile.org/jquery/3.6.0/jquery.min.js
// @grant      GM_addStyle
// @grant      window.onurlchange
// @grant      unsafeWindow
// @grant      GM_openInTab
// @grant      GM_notification
// @grant      GM_registerMenuCommand
// @grant      GM_setValue
// @grant      GM_getValue
// @namespace  https://greasyfork.org/users/7036
// @license    MIT
// @icon         data:image/gif;base64,R0lGODlhAQABAAAAACH5BAEKAAEALAAAAAABAAEAAAICTAEAOw==
// @grant        none
// ==/UserScript==

function excludeVidoData() {
    'use strict';

    // 获取浏览器语言并选择相应的翻译
    const curLang = navigator.language.slice(0, 2);
    const i18n = {
        'zh': {
            'console': '%c脚本[%s] 反馈：%s\n%s',
            'cacheStoringError': '直接媒体类型（如MP4格式）缓存无效果！',
            'cacheStoringConfirm': '视频切片数据能否缓存？检测方法：刷新页面，已观看视频片段不产生网络流量则可缓存。如果能缓存视频切片，选择确认直接缓存全部视频时段；点取消则按默认缓冲区大小进行缓冲。',
            'ready': '准备就绪！ 待命中.',
            'helpMenuOption': '脚本功能快捷键表',
            'helpBody': `双击：切换（网页）全屏         鼠标中键：快进5秒
P：视频截图    i：切换画中画   M：(停止)缓存视频(hls.js)
← →方向键：快退、快进5秒;   方向键 + shift: 20秒
↑ ↓方向键：音量调节   ESC：退出（网页）全屏
空格键：暂停/播放      N：播放下一集
回车键：切换全屏;      回车键 + shift: 切换网页全屏
C(抖音V)：加速0.1倍  X(抖音S)：减速0.1倍  Z(抖音A)：切换加速状态
D：上一帧     F：下一帧(youtube.com用E键)`
        }
    };

    // 声明常量
    const MSG = i18n[curLang] || i18n.en;
    const w = unsafeWindow || window;
    const { host, pathname: path } = location;
    const d = document, find = [].find;
    let $msg, v, _fp, _fs, by; // document.body

    // GM功能菜单检查函数
    const gmFuncOfCheckMenu = (title, saveName, defaultVal = true) => {
        const r = GM_getValue(saveName, defaultVal);
        if (r) title = '√  ' + title;
        GM_registerMenuCommand(title, () => {
            GM_setValue(saveName, !r);
            location.reload();
        });
        return r;
    };
    // 定义一个UA
    // 获取主域名
    const getMainDomain = host => {
        const a = host.split('.');
        let i = a.length - 2;
        if (/^(com?|cc|tv|net|org|gov|edu)$/.test(a[i])) i--;
        return a[i];
    };
    const ua_chrome = 'Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.3626.121 Safari/537.36';
    const u = getMainDomain(host);
    const cfg = {
        isLive: !1,
        disableDBLClick: !1,
        isClickOnVideo: !1,
        multipleV: !1, //多视频页面
        isNumURL: !1 //网址数字分集
    };
    // 视频缓存对象
    const cacheMV = {
        check() {
            const buf = v.buffered;
            const i = buf.length - 1;
            this.iEnd = buf.end(i);
            return this.mode ? this.iEnd > v.duration - 55 : buf.start(0) >= this.playPos || this.iEnd > v.duration - 55;
        },
        finish() {
            v.removeEventListener('canplaythrough', this.onChache);
            v.currentTime = this.playPos;
            this.chached = !1;
            setTimeout(_ => v.pause(), 99);
            HTMLMediaElement.prototype.play = this.rawPlay;
        },
        onChache() {
            if (this.check()) this.finish();
            else v.currentTime = this.iEnd;
        },
        exec() {
            if (v.src.startsWith('http')) {
                alert(MSG.cacheStoringError);
                return;
            }
            this.mode = confirm(MSG.cacheStoringConfirm);
            this.chached = true;
            v.pause();
            this.rawPlay = HTMLMediaElement.prototype.play;
            HTMLMediaElement.prototype.play = () => new Promise(() => {});
            this.playPos = v.currentTime;
            v.addEventListener('canplaythrough', this.onChache);
            this.check();
            v.currentTime = this.iEnd;
        }
    };

    // 缓存事件
    cacheMV.onChache = cacheMV.onChache.bind(cacheMV);

    // 动作列表，将键盘代码与对应的动作关联
    const actList = new Map();
    actList.set(77, _ => { // M 缓存视频
        cacheMV.chached ? cacheMV.finish() : cacheMV.exec();
    });

    // 判断cfg.isLive是否为true，若为false，则将cfg.isLive设置为host字符串是否以live.开头
    cfg.isLive = cfg.isLive || host.startsWith('live.');

    // 将navigator.plugins设置为长度为0的对象
    Reflect.defineProperty(navigator, 'plugins', {
        get() { return { length: 0 } }
    });

    // 注册一个菜单命令，菜单命令的回调函数为alert函数，参数为MSG.helpBody
    GM_registerMenuCommand(MSG.helpMenuOption, alert.bind(w, MSG.helpBody));

}