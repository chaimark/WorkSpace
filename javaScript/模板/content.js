﻿var f;
if (!window.__idm_init__ && navigator.platform.startsWith("Win") && "html" == document.documentElement.localName) {
	window.__idm_init__ = !0; Array.prototype.includes || (Array.prototype.includes = function (b, a) { return 0 <= this.indexOf(b, a) }); NodeList.prototype[Symbol.iterator] || (NodeList.prototype[Symbol.iterator] = Array.prototype[Symbol.iterator]); HTMLCollection.prototype[Symbol.iterator] || (HTMLCollection.prototype[Symbol.iterator] = Array.prototype[Symbol.iterator]); "undefined" == typeof browser && (browser = chrome); navigator.userAgent.includes("Edge/");
	navigator.userAgent.includes("Firefox/"); navigator.userAgent.includes("OPR/"); var t = Array.prototype.includes, B = Function.call.bind(Array.prototype.slice), C = Function.apply.bind(Array.prototype.push), F = Function.call.bind(Array.prototype.forEach), G = { 16: !0, 17: !0, 18: !0, 45: !0, 46: !0 }, H = t.bind(["video", "audio", "object", "embed"]), I = /(?!)/, M = window.self === window.top, N = window.origin || document.origin || location.origin, O = location.host.split(".").splice(-2, 1).shift().toLowerCase(), P = RegExp("^(?:\\(\\d+\\)\\s+)?(.+?)\\s+[-|@]+\\s*" +
		O + "$", "i"); function J() {
			browser.runtime.Ba ? this.W = browser.runtime.Ba(window) : M && (this.W = 0); this.ba = []; this.X = []; this.m = {}; this.ya(); window.__idm_connect__ = this.ya.bind(this, !0); this.b(1, window, "resize", this.yb); this.b(1, window, "scroll", this.zb); this.b(1, window, "blur", this.rb); this.b(1, window, "keydown", this.Ga, !0); this.b(1, window, "keyup", this.Ga, !0); this.b(1, window, "mousedown", this.tb, !0); this.b(1, window, "mouseup", this.vb, !0); this.b(1, window, "mousemove", this.ub, !0); this.b(1, window, "click", this.sb,
				!0); this.b(1, document, "beforeload", this.pb, !0); this.b(1, document, "DOMContentLoaded", this.Fa); "interactive" != document.readyState && "complete" != document.readyState || this.Fa()
		} f = J.prototype; f.f = 0; f.h = 0; f.da = -1; f.ea = -1; f.La = 1; f.J = 0; f.c = ""; f.ra = ""; f.A = ""; f.ta = I; f.xa = I; f.N = I; f.l = {}; f.ua = I; f.va = I; f.wa = I; f.oa = I; f.ja = ":root:empty"; f.la = ":root:empty"; f.L = ":root:empty"; f.ma = ":root:empty"; f.ia = ":root:empty"; f.na = ":root:empty"; f.ha = ":root:empty"; f.K = ":root:empty"; f.ka = 0; f.M = I; f.qa = I; f.sa = I; f.ya = function (b) {
			this.Ha(-1);
			this.a = b = browser.runtime.connect({ name: (M ? "top" : "sub") + (b ? ":retry" : "") + ("file:" == location.protocol ? ":file" : "") }); this.b(-1, b, "onMessage", this.xb); this.b(-1, b, "onDisconnect", this.wb)
		}; f.wb = function () { browser.runtime.lastError; this.Ha(); this.a = this.Y = null; window.__idm_init__ = !1; window.__idm_connect__ = null }; f.xb = function (b) {
			switch (b.shift()) {
				case 11: this.$a.apply(this, b); break; case 17: this.ab.apply(this, b); break; case 12: this.bb.apply(this, b); break; case 13: this.Ta = b.shift(); break; case 14: this.fb.apply(this,
					b); break; case 15: this.gb.apply(this, b); break; case 16: this.hb.apply(this, b); break; case 18: this.eb.apply(this, b)
			}
		}; f.Bb = function (b, a) {
			switch (this.Z = b) {
				case 1: this.Ua = this.o = !0; this.c = a.shift() || ""; this.ta = RegExp(a.shift() || "(?!)"); this.N = RegExp(a.shift() || "(?!)"); this.l = (a.shift() || "").split(/[|:]/).reduce(Q, {}); this.ua = RegExp(a.shift() || "(?!)"); this.va = RegExp(a.shift() || "(?!)"); this.wa = RegExp(a.shift() || "(?!)"); this.xa = RegExp(a.shift() || "(?!)"); break; case 2: this.o = !0; this.c = a.shift() || ""; this.A = a.shift() ||
					""; this.oa = RegExp(a.shift() || "(?!)"); this.ja = a.shift() || ":root:empty"; this.la = a.shift() || ":root:empty"; a.shift(); this.L = a.shift() || ":root:empty"; this.ma = a.shift() || ":root:empty"; this.ia = a.shift() || ":root:empty"; this.na = a.shift() || ":root:empty"; this.ha = a.shift() || ":root:empty"; this.K = a.shift() || ":root:empty"; this.ka = a.shift() || 0; this.Ma = a.shift(); this.M = RegExp(a.shift() || "(?!)"); break; case 4: this.o = !0; this.c = a.shift() || ""; this.ra = a.shift() || ""; this.A = a.shift() || ""; this.qa = RegExp(a.shift() || "(?!)");
					this.Ra = a.shift(); this.Ka = a.shift(); this.Pa = a.shift(); this.Oa = a.shift(); break; case 3: this.sa = RegExp(a.shift() || "(?!)"); break; case 5: this.o = !0; this.c = a.shift() || ""; this.A = a.shift() || "(?!)"; this.Sa = a.shift(); break; default: this.o = !0, this.c = a.shift() || "", this.pa = a.shift()
			}this.c.includes("^/") && ((b = this.ra) ? b = "https?://" + b.slice(+b.startsWith("^"), -b.endsWith("$") || void 0) : b = N.replaceAll(".", "\\."), this.c = this.c.replaceAll("^/", "^(?:" + b + ")?/"))
		}; f.V = function (b) {
			if (this.Z && this.H(arguments)) switch (this.Z) {
				case 1: this.ob(b);
					break; case 2: this.kb(b); break; case 4: this.lb(b); break; case 3: this.nb(b); break; case 5: this.mb(b)
			}
		}; f.R = function (b) { var a = this.a, c; for (c of document.getElementsByTagName("script")) !c.src && b.test(c.innerText) && a.postMessage([34, null, -1, c.outerHTML]); a.postMessage([34, this.u()]) }; f.Ab = function (b) {
			var a = b.data; if (Array.isArray(a) && b.origin == N) switch (a[0]) {
				case 1229212977: window.postMessage([1229212978, this.Y, this.o, this.Ua, this.c, this.A], "/"); break; case 1229212979: this.a.postMessage([36, a[1], a[2]]); break;
				case 1229212981: this.Xa(a[1], a[2])
			}
		}; f.S = function () { var b = window.devicePixelRatio, a = document.width, c = document.body.scrollWidth; a && c && (b = a == c ? 0 : a / c); return b }; f.s = function (b) { var a = b.g; a || (a = b.g = this.Y << 13 | this.La++, b.setAttribute("__idm_id__", a), this.m[a] = b); return a }; f.C = function (b) { b.j && (window.clearTimeout(b.j), b.j = null); b.v && (b.v.disconnect(), b.v = null); this.i && this.i.unobserve(b); this.G && this.G.unobserve(b); delete this.m[b.g] }; f.O = function (b, a, c, d) {
			if (!b.v) {
				var e = this.s(b); e && (a || c) && ((b.v = new MutationObserver(this.I.bind(this))).observe(b,
					{ attributes: !0, attributeFilter: ["style"] }), this.i && this.i.observe(b), d && this.G && this.G.observe(b), d = this.F(b), this.a.postMessage([41, e, b.localName, b.src || b.data, a, c, d]))
			}
		}; f.Za = function (b, a, c, d, e) {
			var h, m, l, p, g = window.getComputedStyle.bind(window); try {
				var k = 0; var q = document.activeElement; q && H(q.localName) || (q = document.elementFromPoint(this.da, this.ea)) && !H(q.localName) && (q = null); for (var n of document.querySelectorAll("video,audio,object,embed")) {
					var u = n.localName, r, w = "object" == u || "embed" == u; if (w) {
						var v =
							n.type.toLowerCase(); if (v.startsWith("text/") || v.startsWith("image/")) continue; if ("embed" == u && "application/x-shockwave-flash" != v) continue
					} if (e && n.U == e && (h = n)) break; if (r = n.src || n.data) r != b && r != a || (h = n); else if (!w) for (var D of n.getElementsByTagName("source")) if ((r = D.src) && (r == b || r == a) && (h = n)) break; if (h) if ("cover" == g(n).objectFit) { var x = n; h = null } else break; if (!(x || q || m)) {
						if (r) { if ((r == c || r == d) && (m = n)) continue; if ("video" == u && r.startsWith("data:audio/")) continue } var z = n.clientWidth, E = n.clientHeight; if (z &&
							E) { var K = n.getBoundingClientRect(); if (!(0 >= K.right + window.scrollX || 0 >= K.bottom + window.scrollY)) if ("hidden" == g(n).visibility) "video" == u && (p || (p = n)); else { w = z * E; if (k < w && 1.35 * z > E && z < 3 * E) { k = w; var R = n } l || (l = n) } }
					}
				}
			} catch (S) { } (n = h || x || q || m) ? "video" != n.localName && "hidden" == g(n).visibility && (n = null) : n = R || l || p; n && ("embed" != n.localName || n.clientWidth || n.clientHeight || (b = n.parentElement, "object" == b.localName && (n = b)), this.s(n) ? (b = n.v = new MutationObserver(this.I.bind(this)), b.observe(n, { attributes: !0, attributeFilter: ["style"] }),
				b.observe(n.parentNode, { childList: !0 }), this.i && this.i.observe(n)) : n = null); return n
		}; f.Ya = function (b, a, c) { var d = null, e, h = browser.runtime.Ba, m; for (m of document.getElementsByTagName("iframe")) { var l = m.Da; null == l && h && (l = m.Da = h(m)); if (l == b) { d = m; break } else d || !(e = m.src) || e != a && e != c || (d = m) } return d && this.s(d) }; f.F = function (b) {
			var a = null; try {
				var c = window.getComputedStyle(b), d = b.getBoundingClientRect(), e = Math.round(d.width), h = Math.round(d.height); if ("hidden" == c.visibility || b.ib) return e > 5 * h ? null : !1; for (var m =
					Math.round(d.left) + b.clientLeft, l = Math.round(d.top) + b.clientTop, p = this.J, g = d = 0, k = b, q, n; p-- && "block" == c.display && (q = k.parentElement) && "div" == q.localName; k = q, c = n) { n = window.getComputedStyle(q); d -= k.offsetLeft; g -= k.offsetTop; if ("hidden" == n.overflowX) { var u = e - q.offsetWidth - d; 0 < d && (e -= d, m += d); 0 < u && (e -= u); p = 0 } if ("hidden" == n.overflowY) { var r = h - q.offsetHeight - g; 0 < g && (h -= g, l += g); 0 < r && (h -= r); p = 0 } } switch (b.localName) {
						case "video": (b.videoWidth || b.U) && document.contains(b) && (a = !1); if (15 > e || 10 > h) return a; break;
						case "audio": if (1 > e && 1 > h) return a; var w = !0
					}var v = document.documentElement, D = v.scrollWidth || v.clientWidth, x = v.scrollHeight || v.clientHeight; if (m >= D || l >= x || w && !m && !l) return a; 0 == m && l == -x && e == D && h == x && (l = 0); a = [m, l, m + e, l + h, this.S()]
			} catch (z) { } return a
		}; f.u = function () {
			var b = M ? 80 : 90; try { var a = window.top.document.title } catch (c) { } a || (a = (a = document.head.querySelector('meta[property="og:title"]')) && a.getAttribute("content"), b = 70); a && (a = a.replace(/^\(\d+\)/, "").replace(/[ \t\r\n\u25B6]+/g, " "), a = new String(a.trim()),
				a.src = b); return a
		}; f.cb = function () { if (!document.elementsFromPoint) return 0; var b = document.documentElement, a = b.clientWidth || b.offsetWidth; b = (b.clientHeight || window.innerHeight) / 4; var c = document.elementsFromPoint(0, 0), d = 0, e = 0, h = 0, m; for (m of c) if (m.offsetWidth >= a) { var l = window.getComputedStyle(m); .95 <= l.opacity && (c = l.zIndex, "fixed" == l.position ? (l = m.offsetHeight + m.offsetTop, d < l && l < b && (d = l, e = c)) : h < c && (h = c)) } e < h && (d = 0); return d }; f.$a = function (b, a, c, d, e, h, m, l) {
			m && L("text/javascript", m); this.Y = b; if (c) {
				this.W =
				c; try { window.frameElement && (window.frameElement.Da = c) } catch (p) { }
			} this.Ca(e); d && this.Bb(d, B(arguments, 8)); l && (this.b(2, window, "message", this.Ab), L("text/javascript", l)); "loading" == document.readyState ? this.ca = !0 : this.V()
		}; f.ab = function (b) { this.fa && this.f && (this.f = 0, this.ga = window.setTimeout(this.$.bind(this, !1), 3E3)); this.V(!0); b && this.Ca() }; f.Ca = function (b) {
			if (this.H(arguments)) {
				var a = [21, this.Db || location.href, document.referrer]; b && a.push(document.getElementsByTagName("video").length, document.getElementsByTagName("audio").length);
				this.a.postMessage(a)
			}
		}; f.fb = function (b) { var a = b ? this.H(arguments) : 1; if (0 < a || 0 > a && 1 != this.Z) { a = this.u(); var c = this.S(), d = this.cb(); this.a.postMessage([24, a, a && a.src, c, d]) } }; f.hb = function (b, a, c, d, e) { c || (c = this.Ya(a, d, e)); d = (d = c && this.m[c]) && this.F(d); this.a.postMessage([22, b, a, c, d]) }; f.eb = function (b, a) { var c = document.getElementsByTagName("a"), d; for (d of c) try { if (d.href == a) { var e = d.download || null; var h = d.innerText.trim() || d.title || null; break } } catch (m) { } this.a.postMessage([35, b, e, h]) }; f.gb = function (b,
			a, c, d, e, h, m) { if (this.H(arguments)) { var l = [23, b, a, !1], p, g, k; if (a) a && ((p = this.m[a]) || (l[3] = !0)); else { if (p = this.Za(c, d, e, h, m)) { if (this.pa && (g = y(p, this.pa))) var q = 200; else if (g = p.getAttribute("operadetachedviewtitle")) q = 130; l[2] = a = p.g; l[5] = p.localName; l[6] = p.src || p.data } !g && (g = this.u()) && (q = g.src, (k = P.exec(g)) && (g = k[1])); l[7] = g; l[8] = q } p && (g = this.F(p), null != g || document.contains(p) ? l[4] = g : (this.C(p), l[3] = !0)); this.a.postMessage(l) } }; f.bb = function (b, a, c, d) {
				var e = d ? RegExp(d, "i") : null; d = this.Aa(document, e,
					a); var h = document.getElementsByTagName("iframe"), m; for (m of h) try { var l = m.contentDocument; l && !m.src && C(d, this.Aa(l, e, a)) } catch (p) { } b = [27, b, this.W, d.length]; c || (b[4] = d, b[5] = location.href, M && (b[6] = location.href, b[7] = document.title)); this.a.postMessage(b)
			}; f.Aa = function (b, a, c) {
				var d = [], e = {}, h = "", m = "", l = !c; if (c) { var p = b.getSelection(); if (!p || p.isCollapsed && !p.toString().trim()) return d } for (var g of b.getElementsByTagName("a")) if (l || p.containsNode(g, !0)) try {
					var k = g.href; k && !e[k] && a.test(k) && (e[k] = d.push([k,
						2, g.download || null, g.innerText.trim() || g.title])); c && e[k] && (m += g.innerText, m += "\n")
				} catch (n) { } for (g of b.getElementsByTagName("area")) if (l || p.containsNode(g, !0)) try { (k = g.href) && !e[k] && a.test(k) && (e[k] = d.push([k, 2, null, g.alt])) } catch (n) { } for (g of b.getElementsByTagName("img")) if (l || p.containsNode(g, !0)) try { (k = g.src) && !e[k] && a.test(k) && (e[k] = d.push([k, 3, null, "<<<=IDMTRANSMITIMGPREFIX=>>>" + g.alt])), l && g.onclick && (h += g.onclick, h += "\n") } catch (n) { } if (c) {
					if (h = p.toString()) {
						c = this.B(m); for (k of this.B(h)) k &&
							!e[k] && a.test(k) && !c.includes(k) && (e[k] = d.push([k, 1]))
					} for (g of b.getElementsByTagName("textarea")) if (b = g.selectionStart, h = g.selectionEnd, (c = p.isCollapsed && b < h) || p.containsNode(g, !0)) try { var q = c ? g.value.substring(b, h) : g.value; for (k of this.B(q)) k && !e[k] && a.test(k) && (e[k] = d.push([k, 1])) } catch (n) { }
				} else {
					for (g of b.getElementsByTagName("iframe")) try { (k = g.src) && !e[k] && a.test(k) && (e[k] = d.push([k, 4])) } catch (n) { } for (g of b.getElementsByTagName("script")) h += g.innerText, h += "\n"; if (h) for (k of this.B(h)) k &&
						!e[k] && a.test(k) && (e[k] = d.push([k, 5]))
				} return d
			}; f.B = function (b) { if (!this.aa) { var a = "\\b\\w+://(?:[%T]*(?::[%T]*)?@)?[%H.]+\\.[%H]+(?::\\d+)?(?:/(?:(?: +(?!\\w+:))?[%T/~;])*)?(?:\\?[%Q]*)?(?:#[%T]*)?".replace(/%\w/g, function (d) { return this[d] }.bind({ "%H": "\\w\\-\u00a0-\ufeff", "%T": "\\w\\-.+*()$!,%\u00a0-\ufeff", "%Q": "^\\s\\[\\]{}()" })); this.aa = RegExp(a, "gi") } for (var c = []; a = this.aa.exec(b);)c.push(a.shift()); return c }; f.H = function (b) {
				var a = this.f; a || (b = B(b && b.callee ? b : arguments), b.unshift(arguments.callee.caller),
					this.X.push(b)); return a
			}; f.Fa = function (b) { try { var a = window.top.document.getElementsByTagName("title")[0] } catch (c) { } this.f = b && a ? -1 : 1; a && (b = this.fa, b || (this.fa = b = new MutationObserver(this.$.bind(this))), b.observe(a, { childList: !0 })); try { this.i = new ResizeObserver(this.I.bind(this)) } catch (c) { } try { this.G = new IntersectionObserver(this.I.bind(this), { threshold: .2 }) } catch (c) { } this.ca && (this.ca = !1, this.V()); for (b = this.X; a = b.shift();)a.shift().apply(this, a) }; f.I = function (b) {
				if (this.a) for (var a of b) b = a.target,
					b.g && (document.contains(b) ? (null != a.isIntersecting && (b.ib = !a.isIntersecting), b.j && window.clearTimeout(b.j), b.j = window.setTimeout(this.qb.bind(this, b), 200)) : (this.a.postMessage([23, null, b.g, !0]), this.C(b)))
			}; f.qb = function (b) { if (!this.a) return this.C(b); b.j = null; var a = document.contains(b), c; a ? c = this.F(b) : this.C(b); this.a.postMessage([23, null, b.g, !a, c]) }; f.$ = function (b) {
				if (b) window.clearTimeout(this.ga), this.ga = window.setTimeout(this.$.bind(this, !1), 1E3); else {
					0 > this.f ? this.f = 1 : ++this.f; for (var a = this.X; b =
						a.shift();)b.shift().apply(this, b)
				}
			}; f.pb = function (b) { var a = b.target, c = a.localName; H(c) && b.url && (a = this.s(a), this.a.postMessage([25, a, c, b.url])) }; f.Ga = function (b) { !b.repeat && G[b.keyCode] && this.a.postMessage([31, b.keyCode, "keydown" == b.type]) }; f.tb = function (b) { this.Ta && this.a.postMessage([28]); if (0 == b.button) { var a = b.view.getSelection(); a && a.isCollapsed && !a.toString().trim() && (this.h = 1); this.a.postMessage([32, b.button, !0]) } }; f.vb = function (b) {
				if (0 == b.button) {
					this.da = b.clientX; this.ea = b.clientY; this.a.postMessage([32,
						b.button, !1]); var a = b.view.getSelection(); a && this.h && (this.h = a.isCollapsed && !a.toString().trim() ? 0 : 2) && this.a.postMessage([26, b.clientX, b.clientY, this.S()])
				}
			}; f.ub = function () { 2 == this.h && (this.h = 0) }; f.sb = function (b) { b = b.target; "a" == b.localName && this.a.postMessage([39, b.hasAttribute("download"), b.href, b.download]) }; f.zb = function () { this.a.postMessage([29]) }; f.yb = function (b) { b = b.target; this.a.postMessage([30, b.innerWidth, b.innerHeight]) }; f.rb = function () { this.h = 0; this.a.postMessage([33]) }; f.b = function (b,
				a, c, d, e) { var h = B(arguments); h[3] = d.bind(this); this.ba.push(h); 0 > b ? (a = a[c], a.addListener.apply(a, h.slice(3))) : a.addEventListener.apply(a, h.slice(2)) }; f.Ha = function (b) { for (var a = this.ba, c = 0; c < a.length; c++) { var d = a[c][0]; if (!b || b == d) { var e = a.splice(c--, 1).pop(), h = e.splice(0, 2).pop(); 0 > d ? (h = h[e.shift()], h.removeListener.apply(h, e)) : h.removeEventListener.apply(h, e) } } }; f.ob = function (b) {
					if (!b) {
						b = this.a; var a, c, d, e, h = 0; for (l of document.getElementsByTagName("script")) {
							if (l.src) {
								if (e = this.N.exec(l.src)) h |=
									1, this.l[e[1]] ? c = e[1] : a = l.src
							} else if (e = this.xa.exec(l.innerText)) h |= 2, this.l[e[1]] && (d = parseInt(e[1], 10)); else if (this.ta.test(l.innerText)) { h |= 4; var m = l.outerHTML } if (7 == h) break
						} if (d || c) { var l = {}; l[118] = d; l[124] = c; b.postMessage([37, 1, 2, l]) } b.postMessage([34, this.u(), null, m]); a && (m = new XMLHttpRequest, m.responseType = "text", m.timeout = 1E4, m.onreadystatechange = this.Cb.bind(this, a, m), m.open("GET", a, !0), m.send()); this.J = 2
					}
				}; f.Cb = function (b, a) {
					if (4 == a.readyState) {
						var c = this.N.exec(b); if ((b = (b = this.ua.exec(a.response)) &&
							parseInt(b[1] || b[2], 10)) && (!this.l[b] || !this.l[c[1]])) { var d = this.va.exec(a.response); a = this.wa.exec(a.response); if (d && a && d[2] == a[1]) { c = c && c[1]; var e = {}; e[118] = b; e[119] = d[0]; e[120] = a[0]; e[124] = c; this.a.postMessage([37, 1, 1, e]) } }
					}
				}; f.kb = function d(a, c) {
					a ? (c && a.shift().disconnect(), this.T.disconnect(), this.Ja.disconnect(), this.Ia.clear(), this.D.clear()) : (this.R(this.oa), this.T = new MutationObserver(this.Wa.bind(this)), this.Ja = new MutationObserver(this.Va.bind(this)), this.Ia = new Set, this.D = new Set, this.J =
						3); if (c = this.Na = document.querySelector(this.la)) { if (a = this.T, a.observe(c, { childList: !0 }), !this.za(c, !0) && (c = c.querySelector(this.ma))) { var e = A(c, this.L); e && a.observe(c, { childList: !0 }); a.observe(e || c.parentNode, { childList: !0 }) } } else !a && (c = document.querySelector(this.ja)) && (e = new MutationObserver(d.bind(this, a = [])), a.push(e), e.observe(c, { childList: !0 }))
				}; f.za = function (a, c) {
					var d = this.T, e = this.L, h = this.ia, m = this.Ja, l = this.Ia, p = 0, g; for (g of a.querySelectorAll(h)) l.has(g) || g.querySelector(h) || (l.add(g),
						m.observe(g, { childList: !0, subtree: !0 }), c && (a = A(g, e)) && (d.observe(a, { childList: !0 }), p++), F(g.getElementsByTagName("video"), this.P.bind(this))); return p
				}; f.Wa = function (a) { var c = this.Na, d; for (d of a) { a = d.target.isSameNode(c); for (var e of d.addedNodes) 1 == e.nodeType && this.za(e, a) } }; f.Va = function (a) { var c = this.D, d = this.K, e; for (e of a) for (var h of e.addedNodes) if (1 == h.nodeType) if (a = h.getElementsByTagName("video"), a.length) F(a, this.P.bind(this)); else for (var m of c) (a = A(m, d)) && a.contains(h) && this.P(m) }; f.P =
					function (a) {
						if (!a.g) {
							var c = this.ha, d = this.ka, e = this.D, h = t.bind(["div", "span", "a"]), m, l, p, g, k = A(a, this.K), q = k && k.querySelector(":scope >" + c); for (d = q ? 0 : d; d-- && k && h(k.localName); k = k.parentNode) {
								var n = k; if (q = A(k, c + ":scope:first-child:only-of-type")) break; if ((l = k.previousSibling) && "div" == l.localName) {
									m || (m = k.querySelector('div[role="slider"]')); 1 == (g = k.querySelectorAll(c)).length ? q = g[0] : (p = k.nextSibling) && 1 == (g = p.querySelectorAll("ul:scope " + c)).length ? q = g[0] : m && 1 < (g = l.querySelectorAll(c)).length && (q =
										g[g.length - 1]); break
								}
							} q ? q = q.href : n && 1 == (g = n.querySelectorAll(this.na)).length && (q = g[0].href); if (q) { if (c = this.M.exec(q)) q = null, c = c[1]; e.delete(a); a.U = q || c; this.O(a, c, q) } else e.add(a), a = this.s(a), e = this.Ma, a && e && window.postMessage([1229212980, a, e], "/")
						}
					}; f.Xa = function (a, c) { if (a = this.m[a]) { if (null == c) { if (1 < document.getElementsByTagName("video").length || !(c = this.M.exec(location.href))) return; c = c[1] } a.U = c; this.D.delete(a); this.O(a, c) } }; f.lb = function (a) {
						if (!a) {
							this.R(this.qa); a = this.Ka; var c = new MutationObserver(this.jb.bind(this));
							c.observe(document.body, { childList: !0 }); var d = this.Qa = y(document, this.Ra); if (d) { c.observe(d, { childList: !0 }); for (var e of d.children) for (e of y(e, a, [])) c.observe(e, { childList: !0 }); F(d.getElementsByTagName("video"), this.Ea.bind(this)) }
						}
					}; f.jb = function (a, c) {
						var d = document.body, e = this.Qa, h = this.Ka, m = this.Pa, l = this.Oa, p; for (p of a) {
							a = p.target.isSameNode(d); var g = p.target.isSameNode(e), k, q; for (q of p.addedNodes) if (1 == q.nodeType) {
								if (g) for (k of y(q, h, [])) c.observe(k, { childList: !0 }); else (k = y(q, a ? l : m)) && c.observe(k,
									{ childList: !0 }); F(q.getElementsByTagName("video"), this.Ea.bind(this))
							}
						}
					}; f.Ea = function (a) { var c = a.getAttribute("poster"); c && this.O(a, null, c, !0) }; f.nb = function (a) { a || this.R(this.sa) }; f.mb = function (a) { if (!a) { var c = y(document, this.Sa); c && (a = this.u(), c = c.outerHTML, this.a.postMessage([34, a, null, c.slice(0, c.indexOf(">") + 1)])) } }; function L(a, c) { var d = a.startsWith("image/") ? "img" : "script"; d = document.createElement(d); d.src = "data:" + a + ";base64," + c; d.onload = d.remove.bind(d); document.documentElement.appendChild(d) }
	function y(a, c, d) {
		function e(k, q) { for (; q && k--;)q = q.parentElement; q && this.push(q) } if (!c) return d; var h, m, l = !0, p = d || [], g = c.split(/ *~(\S) */); 1 == g.length ? (g.unshift(">"), "<" == c[0] && g.splice(1, 1, c.slice(1) + "<")) : g.splice(0, 1); for (; a && (h = g.shift());) {
			c = g.shift(); for (m = 1; "<" == c[c.length - m]; m++); --m && (c = c.slice(0, -m).trim()); switch (h) {
				case "<": for (a = a.closest(c); a && m--;)a = a.parentElement; break; case ">": for (a = a.querySelector(c); a && m--;)a = a.parentElement; break; case "*": a.querySelectorAll(c).forEach(e.bind(p,
					m)); l = !1; break; case "+": e.call(p, m, ":scope" == c ? a : a.querySelector(c)); l = !1; break; case "=": if (c = a.getAttribute(c)) return c; l = !1; break; case "@": if (c = a.innerText.trim()) return c; l = !1
			}
		} return p.length ? p : l && a || d
	} function A(a, c) { var d; for (d = 0; "<" == c[d]; d++); d && (c = c.slice(d)); for (a = a.closest(c); a && d--; a = a.parentElement); return a } function Q(a, c) { c && (a[c] = !0); return a } String.prototype.replaceAll || (String.prototype.replaceAll = function (a, c) { return this.split(a).join(c) }); new J
} !0;
