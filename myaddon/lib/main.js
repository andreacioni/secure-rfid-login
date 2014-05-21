//Main Plugins functions

const user_login = "https://www.portaleargo.it/argoweb/scuolanext/common/login_*";
const teacher_login = "https://www.portaleargo.it/argoweb/famiglia/common/login_form2*";

var pageMod = require("sdk/page-mod");


//Listen for page loading
pageMod.PageMod({
  include: user_login,
  contentScriptFile: [self.data.url("jquery-1.7.min.js"), self.data.url("my-script.js")]
});
