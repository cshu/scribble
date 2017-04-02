package griddoor.iiswingui;

import griddoor.iiswingui.IISUI.BI;

public class SensInvertV extends SensDbacni {

	public SensInvertV(BI syn, CharSequence directIdTable) {
		super(" <InvertV:"+syn.a+"@"+directIdTable+"> ",syn);
	}
	
	@Override
	public SensDbacni MakeSensX(BI syn, CharSequence directIdTable) {
		return new SensInvertV(syn,directIdTable);
	}
	
	@Override
	public boolean instanceofT(Selectable sel){
		return sel instanceof SensInvertV;
	}
	
	
}
