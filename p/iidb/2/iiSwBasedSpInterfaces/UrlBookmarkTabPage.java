package griddoor.iiSwBasedSpInterfaces;

import java.awt.Desktop;
import java.awt.HeadlessException;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.io.IOException;
import java.net.URISyntaxException;
import java.util.ArrayList;
import java.util.logging.Level;

import griddoor.IIPropSets.SingleInstanceSense;
import griddoor.SwingBasedEditorUI.ButtonWithFixedText;
import griddoor.SwingBasedEditorUI.ButtonWithVal;
import griddoor.SwingBasedEditorUI.ComponentGroup;
import griddoor.SwingBasedEditorUI.JStrTextAreaWithLabel;
import griddoor.SwingBasedEditorUI.JavaStringTextArea;
import griddoor.SwingBasedEditorUI.NewTabPageLink;
import griddoor.SwingBasedEditorUI.TextTabPage;
import griddoor.SwingBasedEditorUI.UIContext;
import griddoor.util.EnvironmentUtil;
import griddoor.util.UtilFuncs;

public class UrlBookmarkTabPage extends TextTabPage {

	public UrlBookmarkTabPage(UIContext context) {
		super(context);
		components.add(new JStrTextAreaWithLabel(this, "Search URL bookmarks"){
			@Override
			public void bindStrFunc() {
				super.bindStrFunc();
				if(this.javaStr.isEmpty())
					return;
				ArrayList<SingleInstanceSense> sisarr = new ArrayList<>();
				try {
					Globals.getOuStre().write(Globals.SEARCH_URLBOOKMARK);
					UtilFuncs.outputStreamSendStrCriterionNEscChar(Globals.getOuStre(), javaStr);
					while(true){
						SingleInstanceSense sis = SingleInstanceSense.recvUrlBookmarkInstance();
						if(sis==null)
							break;
						sisarr.add(sis);
					}
				} catch (IOException e) {
					Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
					showMsgBox("Exception thrown when trying to search");
					this.cg.context.redrawNeeded = true;
					return;
				}
				if(sisarr.size()==0){
					showMsgBox("No bookmark found");
					this.cg.context.redrawNeeded = true;
					return;
				}
				this.cg.modalD = new ComponentGroup(this.cg);
				for(SingleInstanceSense sis : sisarr){
					this.cg.modalD.components.add(new ButtonWithVal<SingleInstanceSense>(this.cg.modalD, sis){
						@Override
						public void fillByPrepending() {
							fillEditorWithLines(this.val.gets(1)+'\n');
						}
						@Override
						public void spacePressed() {
							try {
								this.cg.context.editor.addTabNSwitchToIt(new UrlBookmarkEdit(this.cg.context,this.val.id));
							} catch (IOException e) {
								Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
								showMsgBox("Exception thrown when trying to open edit tab");
								this.cg.context.redrawNeeded = true;
							}
						}
						@Override
						public void xPressed() {
							try {
								if(SingleInstanceSense.deleteUrlBookmark(this.val.id)==0){
									this.cg.components.remove(this.cg.cursorIndex);
									this.cg.components.remove(this.cg.cursorIndex);
									if(this.cg.cursorIndex>=this.cg.components.size()){
										if(this.cg.components.isEmpty())
											this.cg.closeItself();
										else
											this.cg.cursorIndex = this.cg.components.size()-1;
									}
								}else{
									showMsgBox("Deletion failed at server side");
								}
							} catch (IOException e) {
								Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
								showMsgBox("Exception thrown when trying to delete");
							}
							this.cg.context.redrawNeeded = true;
						}
					});
					this.cg.modalD.components.add(new ButtonWithVal<SingleInstanceSense>(this.cg.modalD, sis){
						@Override
						public void spacePressed() {
							String uristr = this.val.gets(0);
							if(!uristr.startsWith("http://") && !uristr.startsWith("ftp://") && !uristr.startsWith("https://"))
								uristr = "http://"+uristr;
							try {
								Desktop.getDesktop().browse(new java.net.URI(uristr));
							} catch (IOException | URISyntaxException e) {
								Globals.getLogger().log(Level.WARNING, e.getMessage(), e);
								showMsgBox("Exception thrown when trying to browse");
								this.cg.context.redrawNeeded = true;
							}
						}
						@Override
						public void fillByPrepending() {
							fillEditorWithLines(this.val.gets(0)+'\n');
						}
					});
				}
				this.cg.context.redrawNeeded = true;
			}
		});
		components.add(new ButtonWithFixedText(this, "Make new bookmark"){
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
				this.cg.context.editor.addTabNSwitchToIt(new UrlBookmarkEdit(this.cg.context,strfromcli));
			}
		});
		components.add(new ButtonWithFixedText(this, "Make new bookmark without checking clipboard"){
			@Override
			public void spacePressed() {
				this.cg.context.editor.addTabNSwitchToIt(new UrlBookmarkEdit(this.cg.context));
			}
		});
	}

	public static NewTabPageLink instantiateNewTabPageLink(ComponentGroup cgOfLink){
		return new NewTabPageLink(cgOfLink){

			@Override
			public NewTabPageLink cloneCom(ComponentGroup newcg) {
				return instantiateNewTabPageLink(newcg);
			}

			@Override
			public void fillByPrepending() {
				fillEditorWithLines(tyname+"Url bookmark page in new tab\n");
			}
			@Override
			public void spacePressed() {
				this.cg.context.editor.addTabNSwitchToIt(new UrlBookmarkTabPage(this.cg.context));
			}
		};
	}
}
