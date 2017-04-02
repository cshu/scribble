package griddoor.SwingBasedEditorUI;

import griddoor.util.StringPrepender;

import java.util.Iterator;
import java.util.logging.Logger;

public class Globals {
	//debug next msgid: 1

	
	public static final int maxNumOfLines = 30;//undone
	
	//note this doesn't include the reserved space leading each line and the possible ellipsis trailing each line
	public static final int maxWidthOfLine = 110;//undone
	public static final int maxLengthOfTooltip = 200;
	public static final int maxNumOfLinesOfTooltip = 4;
	
//	public static void makeLineAbbr(StringBuilder sb, String line){
//		if(line.length()>maxWidthOfLine+1)
//			sb.append(line, 0, maxWidthOfLine).append("...\n");
//		else
//			sb.append(line);
//	}
	public static void makeLineAbbr(StringPrepender sp, String line){
		if(line.length()>maxWidthOfLine+1)//this doesn't work when you have chinese characters, and there is \t, and maybe other things
			sp.prepend("...\n").prepend(line, 0, maxWidthOfLine);
		else
			sp.prepend(line);
	}
	
//	public static void makeLeadingCharsForSelected(StringBuilder sb){
//		sb.append('>');
//	}
//	public static void makeLeadingCharsForNotSelected(StringBuilder sb){
//		sb.append(' ');
//	}
	

	public static Iterator<Logger> loggerReq;
	private static Logger logger;
	public static Logger getLogger(){
		if(logger==null){
			logger = loggerReq.next();
		}
		return logger;
	}

}
