package griddoor.iiSwBasedSpInterfaces;

import java.io.IOException;
import java.util.ArrayList;
import java.util.logging.Level;

import griddoor.IIPropSets.EntailmentSet;
import griddoor.IIPropSets.Verb;
import griddoor.SwingBasedEditorUI.ButtonWithFixedText;
import griddoor.SwingBasedEditorUI.ComponentGroup;
import griddoor.SwingBasedEditorUI.JavaStringTextArea;
import griddoor.SwingBasedEditorUI.ModalGroup;
import griddoor.SwingBasedEditorUI.SelectWithMutatingSrc;
import griddoor.util.DependentCounter;

public class ImmutableCgVerb extends ComponentGroup {
	public static final int ADDARG = 0;
	public static final int ARGS = 1;
	public static final int ADDENTSET = 2;
	public static final int ENTSETS = 3;
	public static final int SUBMIT = 4;
	public final DependentCounter dcount = new DependentCounter(5);
	public ImmutableCgVerb(ComponentGroup cg) {
		super(cg);
		components.add(new ButtonWithFixedText(this, "Add arg"){
			@Override
			public void spacePressed() {
				if(true)
					return;//debug only allow single arg for now
				addArg();//note using the enclosing instance
				this.cg.context.redrawNeeded = true;
			}
			//note cloneCom is not needed!
		});
		dcount.Increment(ADDARG);
		addArg();
		components.add(new ButtonWithFixedText(this, "Add entailments set"){
			@Override
			public void spacePressed() {
				addEntSet();//note using the enclosing instance
				this.cg.context.redrawNeeded = true;
			}
			//note cloneCom is not needed!
		});
		dcount.Increment(ADDENTSET);
		components.add(new ButtonWithFixedText(this, "SUBMIT"){
			@Override
			public void spacePressed() {
				String arg0desc = this.cg.components.get(dcount.getBeginIndOf(ARGS)).getJavaStringFromThisAsTextArea();
				if(griddoor.util.UtilFuncs.isNullOrWhiteSpace(arg0desc)){
					showMsgBox("You must specify arg description");
					this.cg.context.redrawNeeded = true;
					return;
				}
				Verb newv = Verb.makeNewVerbWithOneArg();
				newv.args.get(0).desc = arg0desc;
				if(dcount.value[ENTSETS]!=0){
					newv.entailmentSets = new ArrayList<>();
					dcount.selectInd(ENTSETS);
					for(int i=dcount.begin; i<dcount.end; i++){
						ModalGroup mg =(ModalGroup)this.cg.components.get(i);
						EntailmentSet es = new EntailmentSet();
						switch(mg.fixedText){
						case "entailments set`Round-robin mutual exclusions":
							es.formulatype = EntailmentSet.ROUNDROBINMUTUALEXCLUSIONS;
							break;
						case "entailments set`Mass verbs entail this one":
							es.formulatype = EntailmentSet.MASSVERBSEntailTHISONE;
							break;
						case "entailments set`Mass verbs entailed by this one":
							es.formulatype = EntailmentSet.MASSVERBSENTAILEDByTHISONE;
							break;
						default:
							showMsgBox("You must specify entailment(s)");
							this.cg.context.redrawNeeded = true;
							return;
						}
						int intBuf0 = mg.savedModal.components.size();
						if(intBuf0==3){
							showMsgBox("You must add entailment verb(s)");
							this.cg.context.redrawNeeded = true;
							return;
						}
						intBuf0-=2;
						es.verbs = new ArrayList<>();
						for(int j=1; j<intBuf0; j++){
							Verb v = ((Verb)((SelectWithMutatingSrc<?>)mg.savedModal.components.get(j)).selectedItem);
							if(v==null){
								showMsgBox("You must specify entailment verb(s)");
								this.cg.context.redrawNeeded = true;
								return;
							}
							es.verbs.add(v.id);
						}
						newv.entailmentSets.add(es);
					}
				}
				try {
					if(newv.submitNewVerb()!=0){
						Globals.getLogger().log(Level.SEVERE, "Server failed to create verb");
						this.cg.context.redrawNeeded = true;
						showMsgBox("Server failed to create verb");
						return;
					}
				} catch (IOException e) {
					Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
					showMsgBox("Exception thrown when trying to submit verb");
					this.cg.context.redrawNeeded = true;
					return;
				}
				//undone change this cg to an editing verb cg?
				showMsgBox("New verb created.");
				this.cg.context.redrawNeeded = true;
			}
			//note cloneCom is not needed!
		});
		dcount.Increment(SUBMIT);
		this.context.redrawNeeded = true;
	}
	public void addArg(){
		components.add(dcount.getEndIndOf(ARGS),new JavaStringTextArea(this){
			@Override
			public void fillByPrepending() {
				fillEditorWithLines(tyname+"arg`"+javaStr+"\n");
			}
			@Override
			public void bindStrFunc() {
				super.bindStrFunc();
				this.cg.context.redrawNeeded = true;
			}
		});
		dcount.Increment(ARGS);
	}
	public void addEntSet(){
		components.add(dcount.getEndIndOf(ENTSETS), new ModalGroup(this,"entailments set`"){
			@Override
			public void spacePressed() {
				if(this.savedModal == null){
					EntailmentSetComponentGroup modalDToUse = new EntailmentSetComponentGroup(this);
					this.cg.modalD = modalDToUse;
					cg.context.redrawNeeded = true;
				}else{
					super.spacePressed();
				}
			}
			//note cloneCom is note needed!
		});
		dcount.Increment(ENTSETS);
	}

}
