package griddoor.iiSwBasedSpInterfaces;

import java.awt.HeadlessException;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.io.IOException;
import java.net.UnknownHostException;
import java.util.logging.Level;

import griddoor.IIPropSets.SingleInstanceSense;
import griddoor.SwingBasedEditorUI.ButtonWithFixedText;
import griddoor.SwingBasedEditorUI.ButtonWithVal;
import griddoor.SwingBasedEditorUI.ComponentGroupCanBeClosedWithEsc;
import griddoor.SwingBasedEditorUI.JStrTextAreaWithSynopsis;
import griddoor.SwingBasedEditorUI.TextTabPage;
import griddoor.SwingBasedEditorUI.UIContext;
import griddoor.util.EnvironmentUtil;
import griddoor.util.UtilFuncs;

public class UrlBookmarkEdit extends TextTabPage {

	public long snId;
	
	public UrlBookmarkEdit(UIContext context, String strfromcli){
		super(context);
		String[] strs = parseStrFromCli(strfromcli);
		JStrTextAreaWithSynopsis descjstr = new JStrTextAreaWithSynopsis(this,strs[0]);
		JStrTextAreaWithSynopsis urljstr = new JStrTextAreaWithSynopsis(this,strs[1]);
		components.add(descjstr);
		components.add(urljstr);
		addComs(descjstr,urljstr);
	}
	
	public UrlBookmarkEdit(UIContext context){
		super(context);
		JStrTextAreaWithSynopsis descjstr = new JStrTextAreaWithSynopsis(this);
		JStrTextAreaWithSynopsis urljstr = new JStrTextAreaWithSynopsis(this);
		components.add(descjstr);
		components.add(urljstr);
		addComs(descjstr,urljstr);
	}
	
	public UrlBookmarkEdit(UIContext context, long id) throws UnknownHostException, IOException {
		super(context);
		snId = id;
		SingleInstanceSense sis = SingleInstanceSense.selectUrlBookmarkInstance(snId);
		JStrTextAreaWithSynopsis descjstr = new JStrTextAreaWithSynopsis(this, sis.gets(1));
		JStrTextAreaWithSynopsis urljstr = new JStrTextAreaWithSynopsis(this, sis.gets(0));
		components.add(descjstr);
		components.add(urljstr);
		Globals.getOuStre().write(Globals.SELECT_SAPROP);
		UtilFuncs.outputStreamSendInt64(Globals.getOuStre(), snId);
		do{
			SingleInstanceSense vsens = SingleInstanceSense.recvVerb();
			if(vsens == null)
				break;
			components.add(new ButtonWithVal<Long>(this,vsens.id){
				@Override
				public void fillByPrepending() {
					fillEditorWithLines(String.valueOf(SingleInstanceSense.allsenses.get(this.val))+'\n');
				}
			});
		}while(true);
		addComs(descjstr,urljstr);
	}

	void addComs(JStrTextAreaWithSynopsis descjstr, JStrTextAreaWithSynopsis urljstr){
		components.add(new ButtonWithFixedText(this, "Edit props"){
			@Override
			public void spacePressed() {
				if(snId==0)
					showMsgBox("This is a new bookmark. You cannot proceed.");
				else
					try {
						this.cg.context.editor.addTabNSwitchToIt(new SingleArgPropsEdit(this.cg.context,snId,"[Bookmark] "+urljstr.javaStr));
					} catch (IOException e) {
						Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
						showMsgBox("Exception thrown when trying to load edit page");
					}
				this.cg.context.redrawNeeded = true;
			}
		});
		components.add(new ButtonWithFixedText(this, "Update (Insert) Database"){
			@Override
			public void spacePressed() {
				if(urljstr.javaStr.isEmpty() || descjstr.javaStr.indexOf('\n')!=-1 || urljstr.javaStr.indexOf('\n')!=-1){
					showMsgBox("Must be one-line, and URL cannot be empty");
				}else{
					try {
						long lbuf = SingleInstanceSense.upsertUrlBookmark(snId, urljstr.javaStr, descjstr.javaStr);
						if(lbuf==0){
							showMsgBox("Failed to complete update/insertion");
						}else{
							snId = lbuf;
							showMsgBox("Saved successfully");
						}
					} catch (IOException e) {
						Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
						showMsgBox("Exception thrown when trying to upsert");
					}
				}
				this.cg.context.redrawNeeded = true;
			}
		});
		components.add(new ButtonWithFixedText(this, "DELETE"){
			@Override
			public void spacePressed() {
				if(snId==0){
					showMsgBox("This is a new word sense group. You cannot delete it.");
				}else{
					this.cg.modalD = new ComponentGroupCanBeClosedWithEsc(this.cg);
					this.cg.modalD.components.add(new ButtonWithFixedText(this.cg.modalD, "Confirm the deletion"){
						@Override
						public void spacePressed() {
							try {
								if(SingleInstanceSense.deleteUrlBookmark(snId)==0){
									this.cg.context.editor.tabs.removeLast();
									this.cg.context.editor.currentTab = this.cg.context.editor.tabs.getLast();
								}else
									showMsgBox("Failed to complete deletion");
							} catch (IOException e) {
								Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
								showMsgBox("Exception thrown when trying to delete");
							}
							this.cg.context.redrawNeeded = true;
						}
					});
				}
				this.cg.context.redrawNeeded = true;
			}
		});

		components.add(new ButtonWithFixedText(this, "Get string from clipboard, parse and fill"){
			@Override
			public void spacePressed() {
				String strfromcli;
				try {
					strfromcli = EnvironmentUtil.getStringFromSystemClipboard();
				} catch (HeadlessException | UnsupportedFlavorException | IOException e) {
					Globals.getLogger().log(Level.WARNING, e.getMessage(), e);
					showMsgBox("Exception thrown when trying to get clipboard");
					this.cg.context.redrawNeeded = true;
					return;
				}
				String[] strs = parseStrFromCli(strfromcli);
				descjstr.javaStr = strs[0];
				urljstr.javaStr = strs[1];
				this.cg.context.redrawNeeded = true;
			}
		});
	}
	
	static String[] parseStrFromCli(String strfromcli){
		int indoflinefeed = strfromcli.indexOf('\n');
		if(indoflinefeed==-1)
			return new String[]{"",""};
		String firstline = strfromcli.substring(0,indoflinefeed);
		indoflinefeed = strfromcli.indexOf('\n', indoflinefeed+1);
		String secondline;
		if(indoflinefeed==-1)
			secondline = strfromcli.substring(firstline.length()+1);
		else
			secondline = strfromcli.substring(firstline.length()+1,indoflinefeed);
		return new String[]{firstline.replaceAll("\\p{Cntrl}", ""),secondline.replaceAll("\\p{Cntrl}", "")};
	}
}
