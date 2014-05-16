/*var widgets = require("sdk/widget");
var tabs = require("sdk/tabs");
var widget = widgets.Widget({
  id: "mozilla-link",
  label: "Mozilla website",
  contentURL: "http://www.mozilla.org/favicon.ico",
  onClick: function() {
    tabs.open("http://www.mozilla.org/");
  }
});

https://www.portaleargo.it/argoweb/scuolanext/common/login_*
*/

var pageMod = require("sdk/page-mod");
var self = require("sdk/self");

pageMod.PageMod({
  include: "https://www.portaleargo.it/argoweb/famiglia/common/login_form2*",
  contentScriptFile: [self.data.url("jquery-1.7.min.js"), self.data.url("my-script.js")]
});
