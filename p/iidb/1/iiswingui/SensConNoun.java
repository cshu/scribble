package griddoor.iiswingui;

import java.io.IOException;
import java.util.Arrays;

import griddoor.iiswingui.IISUI.BI;
import griddoor.iiswingui.IISUI.II;

public class SensConNoun extends SensDbacni {

	public SensConNoun(BI syn, CharSequence directIdTable) {
		super(" <ConNoun:" + syn.a + "@" + directIdTable + "> ", syn);
	}
	
	@Override
	public SensDbacni MakeSensX(BI syn, CharSequence directIdTable) {
		return new SensConNoun(syn,directIdTable);
	}
	
	@Override
	public boolean instanceofT(Selectable sel){
		return sel instanceof SensConNoun;
	}

}
