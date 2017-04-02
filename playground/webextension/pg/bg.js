browser.runtime.onMessage.addListener(function(message, sender, sendResponse){
	if(sender.tab){
		browser.pageAction.show(sender.tab.id);
		teststr='this is test';
		urlstr=sender.url;
		idstr=sender.tab.id;
	}else{
		sendResponse(''+idstr);
	}
});
