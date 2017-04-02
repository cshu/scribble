package griddoor.SwingBasedEditorUI;

import java.util.ArrayList;

public class ComponentGroup {
	
	//note there 2 kinds of component groups
	//1. edit form
	//when edit form is opened, it's just cloned based on part of its parent.
	//This kind of group needs to overwrite its parent form's data when it's closed.
	//2. one-shot form
	//when such form is opened, it's a newly created object.
	//and when it's closed, everything is disposed, that means the task associated with the form is done.
	//no cloning, no overwriting.
	
	public UIContext context;
	ComponentGroup(UIContext context, ComponentGroup cg){
		this.context = context;
		this.cg = cg;
		components = new ArrayList<>();
	}
	public ComponentGroup(ComponentGroup cg){
		this(cg.context, cg);
	}
	public ComponentGroup(ComponentGroup cg, ArrayList<TextComponent> coms){
		this.context = cg.context;
		this.cg = cg;
		components = coms;
	}
	
	public int cursorIndex;
	public final ArrayList<TextComponent> components;
	public TextComponent getSelectedCom(){
		return components.get(cursorIndex);
	}
	
	public ComponentGroup cg;
	public ComponentGroup modalD;
	public void fillByPrependingAndSetTooltip(){
		if(modalD==null){
			context.charToUseForCursorLine = '>';
			if(!context.uOrDJustPressed && context.editor.currentTab.prependOffset>components.size())
				context.editor.currentTab.prependOffset=0;//navigating to parent dialogs is allowed, but once you press any other key, you fly to the end dialog
		}else{
			modalD.fillByPrependingAndSetTooltip();
			context.sp.prepend(" ++++++++++++++++++++++++++++++++\n");
			context.charToUseForCursorLine = '-';
		}
		if(cursorIndex>=components.size())
			cursorIndex=components.size()-1;//todo you didn't consider unselectable?
		for(int compInd = components.size()-1; compInd>=0; compInd--){
			if(cursorIndex == compInd){
				context.charToUseForLine = context.charToUseForCursorLine;
				components.get(compInd).fillByPrepending();
				context.charToUseForLine = ' ';
			}else
				components.get(compInd).fillByPrepending();
		}
	}
	
	public ComponentGroup cloneGroForSameCg(){
		ComponentGroup valtoret = new ComponentGroup(cg, new ArrayList<TextComponent>(components.size()));
		for(TextComponent com : this.components){
			valtoret.components.add(com.cloneCom(valtoret));
		}
		valtoret.cursorIndex = cursorIndex;
		return valtoret;
	}
	
	public void cursorMoveUp(){
		if(modalD==null){
			for (int i = cursorIndex - 1; i >= 0; i--){
				if (components.get(i).isSelectable()) {
					cursorIndex = i;
					context.redrawNeeded = true;
					break;
				}
			}
		}else
			modalD.cursorMoveUp();
	}
	public void cursorMoveDown(){
		if(modalD==null){
			for (int i = cursorIndex + 1; i < components.size(); i++) {
				if (components.get(i).isSelectable()) {
					cursorIndex = i;
					context.redrawNeeded = true;
					break;
				}
			}
		}else
			modalD.cursorMoveDown();
	}
	public void uPressed(){
		if(this.modalD==null){
			this.context.editor.currentTab.prependOffset+=Globals.maxNumOfLines/2;
			if(this.context.editor.currentTab.prependOffset>this.context.prependLineCount-Globals.maxNumOfLines)
				this.context.editor.currentTab.prependOffset=this.context.prependLineCount-Globals.maxNumOfLines;
			this.cursorIndex-=Globals.maxNumOfLines/2;
			if(this.cursorIndex<0)
				this.cursorIndex = 0;//todo you didn't consider Unselectable?
			this.context.redrawNeeded = true;
		}else{
			this.modalD.uPressed();
		}
		
	}
	public void dPressed(){
		if(this.modalD==null){
			this.context.editor.currentTab.prependOffset-=Globals.maxNumOfLines/2;
			if(this.context.editor.currentTab.prependOffset<0)
				this.context.editor.currentTab.prependOffset=0;
			this.cursorIndex+=Globals.maxNumOfLines/2;
			if(this.cursorIndex>=this.components.size())
				this.cursorIndex=this.components.size()-1;//todo you didn't consider Unselectable?
			this.context.redrawNeeded = true;
		}else{
			this.modalD.dPressed();
		}
	}
	public void xPressed(){
		if(modalD==null)
			getSelectedCom().xPressed();
		else
			modalD.xPressed();
	}
	public void spacePressed(){
		if(modalD==null)
			getSelectedCom().spacePressed();
		else
			modalD.spacePressed();
	}
	public String strForInsertMode(){
		if(modalD==null)
			return getSelectedCom().strForInsMode();
		else
			return modalD.strForInsertMode();
	}
	public void bindStrFunct(){
		if(modalD==null)
			getSelectedCom().bindStrFunc();
		else
			modalD.bindStrFunct();
	}
	public void escPressed(){
		if(modalD==null){
			ComponentGroupCanBeClosedWithEsc modalDToUse = new ComponentGroupCanBeClosedWithEsc(this);
			modalDToUse.components.add(new ButtonWithFixedText(modalDToUse,"DISCARD AND CLOSE"){
				@Override
				public void spacePressed() {
					this.cg.closeItsParent();
					this.cg.context.redrawNeeded = true;
				}
			});
			modalD = modalDToUse;
			this.context.redrawNeeded = true;
		}else
			modalD.escPressed();
	}
	public void closeItself(){
		cg.modalD = null;
	}
	public void closeItsParent(){
		cg.cg.modalD = null;
	}
	public void sitOnModalD(){
		cg.modalD = this;
	}
}
