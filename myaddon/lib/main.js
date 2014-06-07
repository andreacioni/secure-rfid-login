//Main Plugins functions

const teacher_login = "https://www.portaleargo.it/argoweb/scuolanext/common/login_*";
const user_login = "https://www.portaleargo.it/argoweb/famiglia/common/login_form*";

var pageMod = require("sdk/page-mod");
var self = require("sdk/self");

//Listen for page loading
pageMod.PageMod({
  include: teacher_login,
  contentScriptFile: [self.data.url("jquery-1.7.min.js"), self.data.url("my-script.js")]
});
