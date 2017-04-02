package griddoor.iiswingui;

import griddoor.iiswingui.IISUI.BI;

public class OptWithTypeAndBI extends OptWithBI {

	public boolean isInverseVerb;
	public String opttext;
	
	public OptWithTypeAndBI(BI syn, String directIdTableOrDesc, boolean isInverseVerb) {
		super(syn);
		this.isInverseVerb = isInverseVerb;
		this.opttext = "<:"+syn.a+":"+directIdTableOrDesc+">"; 
		//todo: add part of speech
	}

	@Override
	public int length() {
		return opttext.length();
	}

	@Override
	public char charAt(int index) {
		return opttext.charAt(index);
	}

	@Override
	public CharSequence subSequence(int start, int end) {
		return opttext.subSequence(start, end);
	}
	
	@Override
	public String toString(){
		return opttext;
	}

}
