package griddoor.iiswingui;

import griddoor.iiswingui.IISUI.BI;

public class SensAbsNoun extends SensDbacni {

	public SensAbsNoun(BI syn, CharSequence directIdTable) {
		super(" <AbsNoun:" + syn.a + "@" + directIdTable + "> ", syn);
	}
	
	@Override
	public SensDbacni MakeSensX(BI syn, CharSequence directIdTable) {
		return new SensAbsNoun(syn,directIdTable);
	}
	
	@Override
	public boolean instanceofT(Selectable sel){
		return sel instanceof SensAbsNoun;
	}
}
