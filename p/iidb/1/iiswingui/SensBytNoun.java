package griddoor.iiswingui;

import griddoor.iiswingui.IISUI.BI;

public class SensBytNoun extends SensDbacni {

	public SensBytNoun(BI syn, CharSequence directIdTable) {
		super(" <BytNoun:"+syn.a+"@"+directIdTable+"> ",syn);
	}
	
	@Override
	public SensDbacni MakeSensX(BI syn, CharSequence directIdTable) {
		return new SensBytNoun(syn,directIdTable);
	}
	
	@Override
	public boolean instanceofT(Selectable sel){
		return sel instanceof SensBytNoun;
	}

}
