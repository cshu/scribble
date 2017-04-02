browser.runtime.sendMessage(1).then(function(responsemsg){
	document.getElementsByTagName('input')[0].value=responsemsg;
},function(error){
});
