package griddoor.iiswingui;

import griddoor.iiswingui.IISUI.BI;

public class SensNoninvV extends SensDbacni {

	public SensNoninvV(BI syn, CharSequence directIdTable) {
		super(" <NoninvV:"+syn.a+"@"+directIdTable+"> ",syn);
	}
	
	@Override
	public SensDbacni MakeSensX(BI syn, CharSequence directIdTable) {
		return new SensNoninvV(syn,directIdTable);
	}
	
	@Override
	public boolean instanceofT(Selectable sel){
		return sel instanceof SensNoninvV;
	}
	


}
