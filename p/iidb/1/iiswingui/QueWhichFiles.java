package griddoor.iiswingui;

import java.awt.event.KeyEvent;
import java.io.File;

import javax.swing.JOptionPane;

public class QueWhichFiles extends QueToAnsWithStr {

	public QueWhichFiles() {
		super("Which file(s)? (Input path)");
	}
	
	@Override
	public void FuncKeyPressed(KeyEvent e) {
		if (e.getKeyCode() == KeyEvent.VK_SPACE) {
			if (IISUI.tcFil.queFiles!=null && IISUI.tcFil.queFiles.size()>0)
				if(JOptionPane.showConfirmDialog(IISUI.theJfr, "Show the File Console? Or cancel to do a new search.", "There are still files listed in console", JOptionPane.OK_CANCEL_OPTION) == JOptionPane.OK_OPTION)
				{
					IISUI.tc = IISUI.tcFil;
					IISUI.tc.CalcSelectionPos();
					IISUI.tc.DrawTextAndSelection();
					return;
				}
			if(strans==null)
				return;
			File fileObj = Utils.GetFileFromReadablePath(strans);
			if (fileObj.isDirectory()) {
				File[] items = fileObj.listFiles();
				if(items.length==0){
					IISUI.sysOut.atAlias.append("\nEmpty directory.");
					IISUI.tc.DrawTextAndSelection();
					return;
				}
				for(File item : items)
					if(!item.isFile()){
						IISUI.sysOut.atAlias.append("\nNot all child items are files, this action is unsupported.");
						IISUI.tc.DrawTextAndSelection();
						return;
					}

				IISUI.tc = IISUI.tcFil;
				IISUI.tcFil.title = fileObj.getPath();
				IISUI.tcFil.setFilesAndRefr(items);
				//undone, switch to files manipulation mode
			} else if (fileObj.isFile()) {
				//undone, switch to files manipulation mode
				
				//IISUI.sysOut.atAlias.append("\n" + fileObj.length() + " bytes, modified at " + fileObj.lastModified());
				//addQueAndSetItAsCurrentAndReCalcSelectionPosAndSet2ndLastParag(new QuestionAnsWithOpt("\n     What to do with the file?", new String[] { "Open file location", "Win MoveFile And Register in II", "Only Register in II" }));
				//refreshMainConsole();
				
				

				// Runtime.getRuntime().exec("explorer /select," +
				// fileObj.getCanonicalPath());
				// } catch (IOException e) {
				// e.printStackTrace();
				// }
			} else {
				IISUI.sysOut.atAlias.append("\nNo such File/Directory.");
				IISUI.tc.DrawTextAndSelection();
			}
		}
	}

}
