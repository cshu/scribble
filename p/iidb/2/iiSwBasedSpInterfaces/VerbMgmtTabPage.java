package griddoor.iiSwBasedSpInterfaces;

import java.io.IOException;
import java.util.ArrayList;
import java.util.logging.Level;

import griddoor.IIPropSets.Verb;
import griddoor.IIPropSets.EntailmentSet;
import griddoor.SwingBasedEditorUI.ButtonWithFixedText;
import griddoor.SwingBasedEditorUI.ButtonWithVal;
import griddoor.SwingBasedEditorUI.ComponentGroup;
import griddoor.SwingBasedEditorUI.JavaStringTextArea;
import griddoor.SwingBasedEditorUI.ModalGroup;
import griddoor.SwingBasedEditorUI.NewTabPageLink;
import griddoor.SwingBasedEditorUI.TextComponent;
import griddoor.SwingBasedEditorUI.TextTabPage;
import griddoor.SwingBasedEditorUI.UIContext;
import griddoor.util.SqliteUtil;
import griddoor.util.UtilFuncs;

public class VerbMgmtTabPage extends TextTabPage {

	public VerbMgmtTabPage(UIContext context) {
		super(context);
		
		components.add(new JavaStringTextArea(this, ""){
			@Override
			public void fillByPrepending() {
				fillEditorWithLines(tyname+"Search text elaboration of verbs and args\n");
			}
			@Override
			public void spacePressed() {
				ArrayList<Verb> verbs = new ArrayList<>();
				try {
					Globals.getOuStre().write(Globals.SELECTV);//undone search verbs whose UTF8TEXT contains criteria
					if(javaStr.isEmpty())
						griddoor.util.UtilFuncs.outputStreamSendLen24AndStrUtf8(Globals.getOuStre(), "");
					else
						UtilFuncs.outputStreamSendStrCriterionNEscChar(Globals.getOuStre(), javaStr);
					while(true){
						Verb v = Verb.recvVerbFromSvr();
						if(v==null)
							break;
						verbs.add(v);
					}
				} catch (IOException e) {
					Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
					showMsgBox("Exception thrown when trying to search");
					this.cg.context.redrawNeeded = true;
					return;
				}
				if(verbs.size()==0){
					showMsgBox("No verb found");
					this.cg.context.redrawNeeded = true;
					return;
				}
				//note a one-shot form is shown here
				this.cg.modalD = new ComponentGroup(this.cg);
				for(Verb v : verbs){
					this.cg.modalD.components.add(new ButtonWithVal<Verb>(this.cg.modalD, v){
						public void addEntSet(){
							//undone is this needed?
						}
						@Override
						public void spacePressed() {
							this.cg.modalD = new ComponentGroup(this.cg);
							this.cg.modalD.components.add(new ButtonWithFixedText(this.cg.modalD, "Add as candidate"){
								@Override
								public void spacePressed() {
									Verb.candidates.add((Verb)((ButtonWithVal<?>)this.cg.cg.getSelectedCom()).val);
									this.cg.closeItself();
									this.cg.context.redrawNeeded = true;
								};
							});
							this.cg.modalD.components.add(new JavaStringTextArea(this.cg.modalD, this.val.desc){
								@Override
								public void fillByPrepending() {
									fillEditorWithLines(tyname+"verb text`"+javaStr+"\n");
								}
								@Override
								public void bindStrFunc() {
									super.bindStrFunc();
									this.cg.context.redrawNeeded = true;
								}
							});
							this.cg.modalD.components.add(new ButtonWithFixedText(this.cg.modalD, "Add entailments set"){
								@Override
								public void spacePressed() {
									
									//undone add new entailment set
									this.cg.context.redrawNeeded = true;
								}
								//note cloneCom is not needed!
							});
							for(EntailmentSet es : v.entailmentSets){
								//undone copy all data from es to UI
								String fixedText;
								switch(es.formulatype){
								case EntailmentSet.ROUNDROBINMUTUALEXCLUSIONS:
									fixedText = "entailments set`Round-robin mutual exclusions";
									break;
								case EntailmentSet.MASSVERBSEntailTHISONE:
									fixedText = "entailments set`Mass verbs entail this one";
									break;
								case EntailmentSet.MASSVERBSENTAILEDByTHISONE:
									fixedText = "entailments set`Mass verbs entailed by this one";
									break;
								default:
									//undone unexpected error?
									throw new RuntimeException("unexpected error formula type");//? is this one handled by UncaughtExceptionHandler?
								}
								this.cg.modalD.components.add(this.cg.modalD.components.size()-1,new ModalGroup(this.cg.modalD,fixedText){
									//undone a defined entailment set
								});
							}
							this.cg.modalD.components.add(new ButtonWithFixedText(this.cg.modalD,"SAVE CHANGES"){
								//undone pressed
							});
							this.cg.context.redrawNeeded = true;
						}
					});
				}
				this.cg.context.redrawNeeded = true;
			}
		});
		components.add(instantiateFormulateVerbBtn(this));
	}
	
	public static ButtonWithFixedText instantiateFormulateVerbBtn(ComponentGroup cgOfNewInst){
		return new ButtonWithFixedText(cgOfNewInst, "Formulate a new verb"){
			@Override
			public void spacePressed() {
				this.cg.modalD = new ImmutableCgVerb(this.cg);
				this.cg.context.redrawNeeded = true;
			}
			@Override
			public TextComponent cloneCom(ComponentGroup newcg) {//? unnecessary method?
				return instantiateFormulateVerbBtn(newcg);
			}
		};
	}

	public static NewTabPageLink instantiateNewTabPageLink(ComponentGroup cgOfLink){
		return new NewTabPageLink(cgOfLink){

			@Override
			public NewTabPageLink cloneCom(ComponentGroup newcg) {
				return instantiateNewTabPageLink(newcg);
			}

			@Override
			public void fillByPrepending() {
				fillEditorWithLines(tyname+"Verb Management page in new tab\n");
			}
			@Override
			public void spacePressed() {
				this.cg.context.editor.addTabNSwitchToIt(new VerbMgmtTabPage(this.cg.context));
			}
		};
	}
	
	@Override
	public String toString() {
		return "Verb Management";
	}
}

