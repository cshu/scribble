package griddoor.iiSwBasedSpInterfaces;

import java.util.List;

import griddoor.IIPropSets.Verb;
import griddoor.SwingBasedEditorUI.ButtonWithFixedText;
import griddoor.SwingBasedEditorUI.ComponentGroup;
import griddoor.SwingBasedEditorUI.ComponentGroupAffiliatedToModalGro;
import griddoor.SwingBasedEditorUI.ModalGroup;
import griddoor.SwingBasedEditorUI.OkButton;
import griddoor.SwingBasedEditorUI.SelectComponent;
import griddoor.SwingBasedEditorUI.SelectWithMutatingSrc;
import griddoor.SwingBasedEditorUI.TextComponent;

public class EntailmentSetComponentGroup extends ComponentGroupAffiliatedToModalGro {
	
	static final String[] ArgEntailOpts = {"(Default) Conjoined arg set0","Externalization","Blank Arg"};

	public static ButtonWithFixedText instantiateAddVerbBtn(ComponentGroup cgOfNewInst){
		return new ButtonWithFixedText(cgOfNewInst, "Add Verb"){
			@Override
			public void spacePressed() {
				//debug for now, only single-arg verb is supported, each verb added is a dull entry, default to conjoining, cannot set any arg
				this.cg.components.add(this.cg.components.size()-2, new SelectWithMutatingSrc<Verb>("Verb",this.cg, Verb.candidates));
				this.cg.context.redrawNeeded = true;
			}
			@Override
			public TextComponent cloneCom(ComponentGroup newcg) {
				return instantiateAddVerbBtn(newcg);
			}
		};
	}
	
	public EntailmentSetComponentGroup(ModalGroup mg) {
		super(mg);
		
		components.add(new ButtonWithFixedText(this, "Round-robin mutual exclusions"){
			@Override
			public void spacePressed() {
				EntailmentSetComponentGroup thisCg = ((EntailmentSetComponentGroup)this.cg);
				thisCg.modGro.fixedText="entailments set`Round-robin mutual exclusions";
				this.cg.components.clear();
				this.cg.cursorIndex = 0;
				for(TextComponent argTc : thisCg.getVerbArgs()){
					this.cg.components.add(new SelectComponent<String>(argTc.strForInsMode(), this.cg, ArgEntailOpts));
				}
				this.cg.components.add(instantiateAddVerbBtn(this.cg));
				this.cg.components.add(new OkButton(this.cg));
				thisCg.modGro.savedModal = thisCg.cloneGroForSameCg();
				this.cg.context.redrawNeeded = true;
			}
			//note cloneCom not needed
		});
		components.add(new ButtonWithFixedText(this, "Mass verbs entail this one"){
			@Override
			public void spacePressed() {
				EntailmentSetComponentGroup thisCg = ((EntailmentSetComponentGroup)this.cg);
				thisCg.modGro.fixedText="entailments set`Mass verbs entail this one";
				this.cg.components.clear();
				this.cg.cursorIndex = 0;
				for(TextComponent argTc : thisCg.getVerbArgs()){
					this.cg.components.add(new SelectComponent<String>(argTc.strForInsMode(), this.cg, ArgEntailOpts));
				}
				this.cg.components.add(instantiateAddVerbBtn(this.cg));
				this.cg.components.add(new OkButton(this.cg));
				thisCg.modGro.savedModal = thisCg.cloneGroForSameCg();
				this.cg.context.redrawNeeded = true;
			}
			//note cloneCom not needed
		});
		components.add(new ButtonWithFixedText(this, "Mass verbs entailed by this one"){
			@Override
			public void spacePressed() {
				EntailmentSetComponentGroup thisCg = ((EntailmentSetComponentGroup)this.cg);
				thisCg.modGro.fixedText="entailments set`Mass verbs entailed by this one";
				this.cg.components.clear();
				this.cg.cursorIndex = 0;
				for(TextComponent argTc : thisCg.getVerbArgs()){
					this.cg.components.add(new SelectComponent<String>(argTc.strForInsMode(), this.cg, ArgEntailOpts));
				}
				this.cg.components.add(instantiateAddVerbBtn(this.cg));
				this.cg.components.add(new OkButton(this.cg));
				thisCg.modGro.savedModal = thisCg.cloneGroForSameCg();
				this.cg.context.redrawNeeded = true;
			}
			//note cloneCom not needed
		});
		components.add(new ButtonWithFixedText(this, "Sophisticated single entailment"){
			@Override
			public void spacePressed() {
				//undone remove itself, then add bunch of things
			}
			//note cloneCom not needed
		});
		
		
		//undone remove
//		components.add(new SelectComponent("formula type`", this, "Round-robin mutual exclusions","Mass verbs entail this one","Mass verbs entailed by this one","Sophisticated single entailment"){
//			//undone
//		});
		
		//undone add components about entailments
		
		
	}
	public List<TextComponent> getVerbArgs(){
		ImmutableCgVerb verbCg = ((ImmutableCgVerb)this.modGro.cg);
		verbCg.dcount.selectInd(ImmutableCgVerb.ARGS);
		return verbCg.components.subList(verbCg.dcount.begin, verbCg.dcount.end);
	}

}
