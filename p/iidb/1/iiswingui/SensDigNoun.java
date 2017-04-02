package griddoor.iiswingui;

import griddoor.iiswingui.IISUI.BI;

public class SensDigNoun extends SensDbacni {

	public SensDigNoun(BI syn, CharSequence directIdTable) {
		super(" <DigNoun:" + syn.a + "@" + directIdTable + "> ", syn);
	}
	
	@Override
	public SensDbacni MakeSensX(BI syn, CharSequence directIdTable) {
		return new SensDigNoun(syn,directIdTable);
	}
	
	@Override
	public boolean instanceofT(Selectable sel){
		return sel instanceof SensDigNoun;
	}

}
