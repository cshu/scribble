package griddoor.iiswingui;

import griddoor.iiswingui.IISUI.BI;

public class OptWithBI implements CharSequence {

	public BI syn;
	public OptWithBI(BI syn) {
		this.syn = syn;
	}

	@Override
	public int length() {
		return syn.a.length();
	}

	@Override
	public char charAt(int index) {
		return syn.a.charAt(index);
	}

	@Override
	public CharSequence subSequence(int start, int end) {
		return syn.a.subSequence(start, end);
	}
	
	@Override
	public String toString(){
		return syn.a;
	}
}
