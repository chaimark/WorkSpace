// 获取主域名
const getMainDomain = host => {
    const a = host.split('.');
    let i = a.length - 2;
    if (/^(com?|cc|tv|net|org|gov|edu)$/.test(a[i])) i--;
    return a[i];
};
const HostURL = getMainDomain(host);

// 视频缓存对象
let cacheMV = {
    var:chached = !1,
    onChache () {
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
   
}
// 缓存事件
cacheMV.onChache = cacheMV.onChache.bind(cacheMV);

// 缓存视频
cacheMV.chached ? cacheMV.finish() : cacheMV.exec();