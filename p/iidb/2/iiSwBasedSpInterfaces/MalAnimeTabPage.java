package griddoor.iiSwBasedSpInterfaces;

import java.awt.Desktop;
import java.awt.HeadlessException;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.io.IOException;
import java.net.URISyntaxException;
import java.util.ArrayList;
import java.util.logging.Level;

import griddoor.IIPropSets.MalAnime;
import griddoor.IIPropSets.SingleInstanceSense;
import griddoor.Myanimelist.Util;
import griddoor.SwingBasedEditorUI.ButtonWithVal;
import griddoor.SwingBasedEditorUI.ComponentGroup;
import griddoor.SwingBasedEditorUI.JavaStringTextArea;
import griddoor.SwingBasedEditorUI.NewTabPageLink;
import griddoor.SwingBasedEditorUI.TextTabPage;
import griddoor.SwingBasedEditorUI.UIContext;
import griddoor.util.SqliteUtil;
import griddoor.util.UtilFuncs;

public class MalAnimeTabPage extends TextTabPage {

	public MalAnimeTabPage(UIContext context) {
		super(context);
		

		components.add(new JavaStringTextArea(this, ""){
			@Override
			public void fillByPrepending() {
				fillEditorWithLines(tyname+"Search for animes on MAL (through clipboard, press space to parse)\n");
			}
			@Override
			public void bindStrFunc(){
				super.bindStrFunc();
				try {
					Desktop.getDesktop().browse(new java.net.URI(Util.getSearchUrlStr(javaStr)));
				} catch (IOException | URISyntaxException e) {
					Globals.getLogger().log(Level.WARNING, e.getMessage(), e);
					showMsgBox("Exception thrown when trying to browse");
					this.cg.context.redrawNeeded = true;
				}
			}
			@Override
			public void spacePressed() {
				MalAnime[] malAnimePropSets;
				try {
					malAnimePropSets = Util.parseClipboardForMalAnimes();
				} catch (HeadlessException | UnsupportedFlavorException | IOException e) {
					Globals.getLogger().log(Level.WARNING, e.getMessage(), e);
					showMsgBox("Exception thrown when trying to parse for mal animes");
					this.cg.context.redrawNeeded = true;
					return;
				}
				//undone add illegalstateexception handling here, this can happen when java cannot open system clipboard
				//undone actually maybe you should handle Exception here directly
				if(malAnimePropSets.length==0){
					showMsgBox("Nothing found after trying to parse for mal animes");
					this.cg.context.redrawNeeded = true;
					return;
				}
				ComponentGroup modcg = new ComponentGroup(this.cg);
				for(MalAnime ma : malAnimePropSets){
					try {
						ma.checkCandidateOrRegistry();
					} catch (IOException e) {
						Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
						showMsgBox("Exception thrown when trying to prepare list");
						this.cg.context.redrawNeeded = true;
						return;
					}
					modcg.components.add(new ButtonWithVal<MalAnime>(modcg, ma){
						@Override
						public void spacePressed() {
							switch(this.val.chkcr){
							case 255:
								MalAnime.candidates.remove(this.val);
								this.val.chkcr = 254;
								break;
							case 254:
								MalAnime.candidates.add(this.val);
								this.val.chkcr = 255;
								break;
							default://todo handle new (1) and conflict (2-253) differently?
								try {
									this.val.upsertToDb();
								} catch (IOException e) {
									Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
									showMsgBox("Exception thrown when trying to upsert");
									this.cg.context.redrawNeeded = true;
									return;
								}
								if(this.val.sensid==0){
									showMsgBox("Server side failed to do upsert!");
									this.cg.context.redrawNeeded = true;
									return;
								}
								this.val.chkcr = 254;
								break;
							}
							this.cg.context.redrawNeeded = true;
						};
					});
				}
				this.cg.modalD = modcg;
				this.cg.context.redrawNeeded = true;
			}
		});

		components.add(new JavaStringTextArea(this, ""){
			@Override
			public void fillByPrepending() {
				fillEditorWithLines(tyname+"Search for animes having file copies in storage devices\n");
			}
			@Override
			public void bindStrFunc(){
				super.bindStrFunc();
				if(javaStr.isEmpty()){
					showMsgBox("Cannot search without criteria");
					this.cg.context.redrawNeeded = true;
					return;
				}
				ArrayList<SingleInstanceSense> malarr = new ArrayList<>();
				try {
					Globals.getOuStre().write(Globals.SEARCH_ANIMEWITHFILE);
					UtilFuncs.outputStreamSendStrCriterionNEscChar(Globals.getOuStre(), javaStr);
					while(true){
						int byt = Globals.getInStre().read();
						if(byt==0)
							break;
						SingleInstanceSense malanime = SingleInstanceSense.recvMalAnimeInstance();
						malarr.add(malanime);
					}
				} catch (IOException e) {
					Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
					showMsgBox("Exception thrown when trying to search");
					this.cg.context.redrawNeeded = true;
					return;
				}
				if(malarr.size()==0){
					showMsgBox("No anime found");
					this.cg.context.redrawNeeded = true;
					return;
				}
				this.cg.modalD = new ComponentGroup(this.cg);
				for(SingleInstanceSense anime : malarr){
					this.cg.modalD.components.add(new ButtonWithVal<SingleInstanceSense>(this.cg.modalD, anime){
						@Override
						public void spacePressed() {
							try {
								this.cg.context.editor.addTabNSwitchToIt(new MalAnimeEdit(this.cg.context,val.id));
							} catch (IOException e) {
								Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
								showMsgBox("Exception thrown when trying to open an anime");
								this.cg.context.redrawNeeded = true;
							}
						};
					});
				}
				this.cg.context.redrawNeeded = true;
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
				fillEditorWithLines(tyname+"MAL anime page in new tab\n");
			}
			@Override
			public void spacePressed() {
				this.cg.context.editor.addTabNSwitchToIt(new MalAnimeTabPage(this.cg.context));
			}
		};
	}
	@Override
	public String toString() {
		return "MAL Anime";
	}
}
