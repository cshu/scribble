

cont = new AudioContext();


function getdt(){
	newf = new Float32Array(arrayBuffer);
	newf0=newf.subarray(0,newf.length/2);
	newf1=newf.subarray(newf.length/2);

	obuf = cont.createBuffer(2,184320,48000);
	cdt0 = obuf.getChannelData(0);
	cdt1 = obuf.getChannelData(1);
	cdt0.set(newf0);
	cdt1.set(newf1);

	sourc = cont.createBufferSource();
	sourc.buffer=obuf;
	sourc.connect(cont.destination);
	sourc.start(0);//start can only exec once? so sourc cannot be reused? but audiobuffer? can that be reused?
}
