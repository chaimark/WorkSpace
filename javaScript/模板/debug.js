﻿(function () {
	function c(a) { try { if ("function" == typeof a && a.hasOwnProperty("toString")) return !0; if ("object" == typeof a) { if ((a instanceof d || e.includes(a.constructor)) && a.hasOwnProperty("toString")) return !0; for (var b of Object.values(Object.getOwnPropertyDescriptors(a))) if (b.get && !b.set && !b.enumerable) return !0 } } catch (h) { } return !1 } var d = Function, f = console.dir.bind(console), g = console.log.bind(console), e = [Object, RegExp, Date]; console.log = function (a, b) {
		"string" == typeof a || (b = a); c(b) ? console.debug("DEBUG LOG",
			typeof b) : g.apply(null, arguments)
	}; console.dir = function (a) { c(a) ? console.debug("DEBUG DIR", typeof a) : f.apply(null, arguments) }; console.table = function () { }; console.clear = function () { }
})();
