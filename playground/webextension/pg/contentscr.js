//document.body.style.border = "5px solid red";
console.log(333);
console.log(browser);
//browser.tabs.onUpdated.addListener(function(tabId, change, tab) {
//  if (change.status == "complete") {
//    browser.pageAction.show(tabId);
//  }
//});
//console.log(111);alert(4);
//browser.pageAction.show(1);
//console.log(browser.runtime.getBackgroundPage().document.title);
browser.runtime.sendMessage(0);
//new XMLSerializer().serializeToString(document.doctype);
console.log('finished');
