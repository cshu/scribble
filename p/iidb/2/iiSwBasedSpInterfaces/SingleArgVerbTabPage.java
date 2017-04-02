package griddoor.iiSwBasedSpInterfaces;

import java.io.IOException;
import java.util.logging.Level;

import griddoor.IIPropSets.SingleInstanceSense;
import griddoor.SwingBasedEditorUI.ButtonWithFixedText;
import griddoor.SwingBasedEditorUI.ButtonWithVal;
import griddoor.SwingBasedEditorUI.CheckBoxWithLabel;
import griddoor.SwingBasedEditorUI.CloseButton;
import griddoor.SwingBasedEditorUI.ComponentGroup;
import griddoor.SwingBasedEditorUI.ComponentGroupCanBeClosedWithEsc;
import griddoor.SwingBasedEditorUI.JStrTextAreaWithLabelNSynopsis;
import griddoor.SwingBasedEditorUI.NewTabPageLink;
import griddoor.SwingBasedEditorUI.TextTabPage;
import griddoor.SwingBasedEditorUI.UIContext;
import griddoor.util.UtilFuncs;

public class SingleArgVerbTabPage extends TextTabPage {

	public SingleArgVerbTabPage(UIContext context) {
		super(context);
		CheckBoxWithLabel sncb = new CheckBoxWithLabel(this,"Where verbs used for Short Note\n");
		CheckBoxWithLabel ancb = new CheckBoxWithLabel(this,"Where verbs used for Animation\n");
		CheckBoxWithLabel wscb = new CheckBoxWithLabel(this,"Where verbs used for Word Sense Group\n");
		CheckBoxWithLabel ubcb = new CheckBoxWithLabel(this,"Where verbs used for Url Bookmark\n");
		components.add(sncb);
		components.add(ancb);
		components.add(wscb);
		components.add(ubcb);
		components.add(new JStrTextAreaWithLabelNSynopsis(this,"Search desc of single-arg verbs`"){
			@Override
			public void bindStrFunc() {
				super.bindStrFunc();
				if(javaStr.isEmpty())
					return;
				ComponentGroup newcg = new ComponentGroup(this.cg);
				try {
					Globals.getOuStre().write(Globals.SEARCH_VERBDESC);
					UtilFuncs.outputStreamSendStrCriterionNEscChar(Globals.getOuStre(), javaStr);
					Globals.getOuStre().write((sncb.checked?0b1:0) | (ancb.checked?0b10:0) | (wscb.checked?0b100:0) | (ubcb.checked?0b1000:0));
					boolean anyVerb = false;
					do{
						SingleInstanceSense vsens = SingleInstanceSense.recvVerb();
						if(vsens == null)
							break;
						anyVerb = true;
						newcg.components.add(new ButtonWithVal<SingleInstanceSense>(newcg,vsens){
							@Override
							public void spacePressed() {
								try {
									this.cg.context.editor.addTabNSwitchToIt(new SingleArgVerbEdit(this.cg.context,this.val.id));
								} catch (IOException e) {
									Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
									showMsgBox("Exception thrown when trying to open SingleArgVerbEdit page");
									this.cg.context.redrawNeeded = true;
								}
							}
						});
					}while(true);
					if(!anyVerb){
						showMsgBox("No verb found");
						this.cg.context.redrawNeeded = true;
						return;
					}
				} catch (IOException e) {
					Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
					showMsgBox("Exception thrown when trying to search verb desc");
					this.cg.context.redrawNeeded = true;
					return;
				}
				this.cg.modalD = newcg;
				this.cg.context.redrawNeeded = true;
			}
		});
		//undone add button to show independent verb families (each one is a separate tree)
		//undone add advanced search for verbs
		//undone add button to show all orphen verbs
		//undone add button to show end node verbs (necessary?)
		//button to show start node verbs
		components.add(new ButtonWithFixedText(this, "Show verbs that don't entail any verbs"){
			@Override
			public void spacePressed() {
				ComponentGroup newcg = new ComponentGroup(this.cg);
				try {
					Globals.getOuStre().write(Globals.SELECTVERBSTHATDONOTENTAIL);
					boolean anyVerb = false;
					do{
						SingleInstanceSense vsens = SingleInstanceSense.recvVerb();
						if(vsens == null)
							break;
						anyVerb = true;
						newcg.components.add(new ButtonWithVal<SingleInstanceSense>(newcg,vsens){
							@Override
							public void spacePressed() {
								try {
									this.cg.context.editor.addTabNSwitchToIt(new SingleArgVerbLineageEdit(this.cg.context,this.val.id));
								} catch (IOException e) {
									Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
									showMsgBox("Exception thrown when trying to open SingleArgVerbLineageEdit page");
									this.cg.context.redrawNeeded = true;
								}
							}
						});
					}while(true);
					if(!anyVerb){
						showMsgBox("No verb found");
						this.cg.context.redrawNeeded = true;
						return;
					}
				} catch (IOException e) {
					Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
					showMsgBox("Exception thrown when trying to select");
					this.cg.context.redrawNeeded = true;
					return;
				}
				this.cg.modalD = newcg;
				this.cg.context.redrawNeeded = true;
			}
		});
		components.add(new JStrTextAreaWithLabelNSynopsis(this,"Insert new single-arg verb`"){
			@Override
			public void spacePressed() {
				try {
					if(javaStr.isEmpty())
						return;
					long newid = SingleInstanceSense.insertSAVerb(javaStr);
					if(newid==0){
						showMsgBox("Insertion failed at server side");
					}else{
						this.cg.modalD = new ComponentGroup(cg);
						this.cg.modalD.components.add(new CloseButton(this.cg.modalD,"Inserted successfully! (Go Back)"));
						this.cg.modalD.components.add(new ButtonWithFixedText(this.cg.modalD,"Open in new tab"){
							@Override
							public void spacePressed() {
								try {
									this.cg.context.editor.addTabNSwitchToIt(new SingleArgVerbEdit(this.cg.context,newid));
								} catch (IOException e) {
									Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
									showMsgBox("Exception thrown when trying to open SingleArgVerbEdit page");
									this.cg.context.redrawNeeded = true;
								}
							}
						});
						this.cg.modalD.components.add(new ButtonWithFixedText(this.cg.modalD,"Open in lineage tab"){
							@Override
							public void spacePressed() {
								try {
									this.cg.context.editor.addTabNSwitchToIt(new SingleArgVerbLineageEdit(this.cg.context,newid));
								} catch (IOException e) {
									Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
									showMsgBox("Exception thrown when trying to open SingleArgVerbLineageEdit page");
									this.cg.context.redrawNeeded = true;
								}
							}
						});
					}
				} catch (IOException e) {
					Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
					showMsgBox("Exception thrown when trying to insert verb");
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
				fillEditorWithLines(tyname+"Single-Arg Verb Management page in new tab\n");
			}
			@Override
			public void spacePressed() {
				this.cg.context.editor.addTabNSwitchToIt(new SingleArgVerbTabPage(this.cg.context));
			}
		};
	}
	
	@Override
	public String toString() {
		return "Single-Arg Verb Management";
	}
}
