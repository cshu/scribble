package griddoor.iiswingui;

import java.io.File;
import java.io.IOException;
import java.nio.channels.FileChannel;
import java.nio.channels.FileLock;
import java.nio.file.Path;
import java.util.Arrays;
import java.util.List;

public class QueFile extends QueToAnsWithOpt {

	public QueFile(File fil) throws IOException {
		super("  "+fil.getName());
		opts = initOpts;
		this.fil = fil;
		path = fil.toPath();
		//filloc = FileChannel.open(path).lock(0, Long.MAX_VALUE, true);
	}
	
	public boolean checked = true;
	public void toggleChecked(){
		checked=!checked;
		allText = (checked?" ":"X")+allText.subSequence(1, allText.length());
	}
	public static int inheritsProduct = 1;

	public static final List<CharSequence> initOpts = Arrays.<CharSequence> asList("(Toggle) Copy to Default Location, AutoRename, Log size, dir, filename, WriteTime (if same size found in DB, warn), and move to REGISTERED folder","inherits nothing", "inherits content of anime","inherits content of text pub/audio product/video product/video game");
	
	//FileLock filloc;
	Path path;
	File fil;

	@Override
	public void FuncSpacePress() {
		String jfTitle = "";
		switch(optans){
		case 0:
			toggleChecked();
			IISUI.tc.DrawTextAndSelection();
			return;
		case 2:
			if(IISUI.tcMal.selectedEntry==null)
				return;
			jfTitle = IISUI.tcMal.selectedEntry.title; 
			
			break;
		case 3:
			//undone
			return;
		}
		inheritsProduct = optans;
		updateJFrameTitle(jfTitle);
		
	}

	public static void updateJFrameTitle(String name) {
		IISUI.theJfr.setTitle(initOpts.get(inheritsProduct)+" "+name);
	}
}
