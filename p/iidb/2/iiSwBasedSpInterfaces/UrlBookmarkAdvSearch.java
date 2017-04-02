package griddoor.iiSwBasedSpInterfaces;

import java.awt.Desktop;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.net.URISyntaxException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.logging.Level;

import griddoor.IIPropSets.SingleInstanceSense;
import griddoor.SwingBasedEditorUI.ButtonWithFixedText;
import griddoor.SwingBasedEditorUI.ButtonWithVal;
import griddoor.SwingBasedEditorUI.ComponentGroup;
import griddoor.SwingBasedEditorUI.ComponentGroupCanBeClosedWithEsc;
import griddoor.SwingBasedEditorUI.JavaStringTextArea;
import griddoor.SwingBasedEditorUI.NewTabPageLink;
import griddoor.SwingBasedEditorUI.TextTabPage;
import griddoor.SwingBasedEditorUI.UIContext;
import griddoor.util.PrimaryStruct;
import griddoor.util.UtilFuncs;

public class UrlBookmarkAdvSearch extends TextTabPage  {

	ArrayList<PrimaryStruct> criter = new ArrayList<>();
	public UrlBookmarkAdvSearch(UIContext context) {
		super(context);
		criter.add(PrimaryStruct.make("and"));
		criter.add(PrimaryStruct.make("d like "));
		criter.add(PrimaryStruct.make("and"));
		criter.add(PrimaryStruct.make("exists "));
		criter.add(PrimaryStruct.make("and"));
		components.add(makeAnd("and"));
		components.add(makeCriter("d like "));
		components.add(makeAnd("and"));
		components.add(makeCriter("exists "));
		components.add(makeAnd("and"));
		components.add(new ButtonWithFixedText(this, "Submit (first and last and/or are ignored)"){
			@Override
			public void spacePressed() {
				this.cg.context.redrawNeeded = true;
				int csi=criter.size();
				StringBuilder slsb = new StringBuilder();
				ByteArrayOutputStream outstre = new ByteArrayOutputStream();
				int numofo = 0;
				for(int i=0;;i+=2){
					PrimaryStruct struc = criter.get(i);
					if(struc==null){showMsgBox("Invalid! LINE: "+i);return;}
					String s = struc.gets().trim();
					int retintbuf = s.lastIndexOf(')');
					numofo-=retintbuf+1;
					if(numofo<0){showMsgBox("Excessive closing parenthesis! LINE: "+i);return;}
					retintbuf = s.indexOf('(', retintbuf+1);
					if(retintbuf!=-1)
						numofo+=s.length()-retintbuf;
					slsb.append(s);
					if(i+1==csi) break;
					struc = criter.get(i+1);
					s = struc.gets();
					try {
						if(s.matches("(?s)\\s*(d|u) like .+")){
							slsb.append(' ').append(s.charAt(s.indexOf('l')-2));
							outstre.write(2);
							if(UtilFuncs.outputStreamSendStrCriterionNEscChar(outstre, s.substring(s.indexOf('l')+5))==0)
								slsb.append(" like ? ");
							else
								slsb.append(" like ? escape ? ");
						}else if(s.matches("(?s)\\s*(d|u) not like .+")){
							slsb.append(' ').append(s.charAt(s.indexOf('n')-2));
							outstre.write(2);
							if(UtilFuncs.outputStreamSendStrCriterionNEscChar(outstre, s.substring(s.indexOf('l')+5))==0)
								slsb.append(" not like ? ");
							else
								slsb.append(" not like ? escape ? ");
						}else if(s.matches("(?s)\\s*exists .+")){
							if(struc.getl()==0L){
								slsb.append(" exists(select * from in_singleargp where s=sp_urlbookmar.i) ");
							}else{
								outstre.write(1);
								UtilFuncs.outputStreamSendInt64(outstre, struc.getl());
								slsb.append(" exists(select * from in_singleargp where v in(with a_a(z)as(values(?)union all select a from in_entailment join a_a on c=z)select z from a_a)and s=sp_urlbookmar.i) ");
							}
						}else if(s.matches("(?s)\\s*not exists .+")){
							if(struc.getl()==0L){
								slsb.append(" not exists(select * from in_singleargp where s=sp_urlbookmar.i) ");
							}else{
								outstre.write(1);
								UtilFuncs.outputStreamSendInt64(outstre, struc.getl());
								slsb.append(" not exists(select * from in_singleargp where v in(with a_a(z)as(values(?)union all select c from in_entailment join a_a on a=z)select z from a_a)and s=sp_urlbookmar.i) ");
							}
						}else{showMsgBox("Invalid! LINE: "+(i+1));return;}
					} catch (IOException e) {
						Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
						showMsgBox("IOException thrown");
						return;
					}
				}
				if(numofo!=0){showMsgBox("Parentheses not closed!");return;}
				outstre.write(0);
				ArrayList<SingleInstanceSense> sisarr = new ArrayList<>();
				try {
					Globals.getOuStre().write(Globals.URLBOOKMARK_VSEARCH);
					UtilFuncs.outputStreamSendLen24AndStrUtf8(Globals.getOuStre(), slsb.substring(slsb.charAt(0)=='a'?3:2, slsb.length()-(slsb.charAt(slsb.length()-1)=='d'?3:2)));
					outstre.writeTo(Globals.getOuStre());
					while(true){
						SingleInstanceSense sis = SingleInstanceSense.recvUrlBookmarkInstance();
						if(sis==null)
							break;
						sisarr.add(sis);
					}
				} catch (IOException e) {
					Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
					showMsgBox("IOException thrown when trying to search.");
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
	}

	JavaStringTextArea makeAnd(String str){
		return new JavaStringTextArea(this,str){
			@Override
			public void fillByPrepending() {
				fillEditorWithLines(this.javaStr+'\n');
			}
			@Override
			public void bindStrFunc() {
				super.bindStrFunc();
				if(cg.context.editor.textInput.getText().matches("\\s*\\)*(and|or)\\(*")){
					criter.set(this.cg.cursorIndex, PrimaryStruct.make(javaStr));
				}else{
					criter.set(this.cg.cursorIndex, null);
				}
				this.cg.context.redrawNeeded = true;
			}
		};
	}


	JavaStringTextArea makeCriter(String initstr){
		return new JavaStringTextArea(this,initstr){
			@Override
			public void fillByPrepending() {
				fillEditorWithLines(UtilFuncs.getFirstLine(this.javaStr)+'\n');
			}
			@Override
			public void bindStrFunc() {
				if(cg.context.editor.textInput.getText().matches("(?s)\\s*(not )?exists .+")){
					String tmpstr = cg.context.editor.textInput.getText();
					ComponentGroupCanBeClosedWithEsc newcg = new ComponentGroupCanBeClosedWithEsc(this.cg);
					newcg.components.add(new ButtonWithFixedText(newcg, "ANY VERB"){
						@Override
						public void spacePressed() {
							javaStr = tmpstr.substring(0,tmpstr.indexOf('e')+"exists ".length())+"ANY VERB";
							criter.set(this.cg.cg.cursorIndex, PrimaryStruct.make(javaStr));
							this.cg.closeItself();
							this.cg.context.redrawNeeded = true;
						}
					});
					try {
						Globals.getOuStre().write(Globals.SEARCH_VERBDESC);
						UtilFuncs.outputStreamSendStrCriterionNEscChar(Globals.getOuStre(), tmpstr.substring(tmpstr.indexOf('e')+"exists ".length()));
						Globals.getOuStre().write(0);
						do{
							SingleInstanceSense vsens = SingleInstanceSense.recvVerb();
							if(vsens == null)
								break;
							newcg.components.add(new ButtonWithVal<SingleInstanceSense>(newcg,vsens){
								@Override
								public void spacePressed() {
									javaStr = tmpstr.substring(0,tmpstr.indexOf('e')+"exists ".length())+this.val.gets(0);
									criter.set(this.cg.cg.cursorIndex, PrimaryStruct.make(this.val.id,javaStr));
									this.cg.closeItself();
									this.cg.context.redrawNeeded = true;
								}
							});
						}while(true);
					} catch (IOException e) {
						Globals.getLogger().log(Level.SEVERE, e.getMessage(), e);
						showMsgBox("Exception thrown when trying to search verb desc");
						this.cg.context.redrawNeeded = true;
						return;
					}
					this.cg.modalD = newcg;
				}else{
					super.bindStrFunc();
					criter.set(this.cg.cursorIndex, PrimaryStruct.make(javaStr));
				}
				this.cg.context.redrawNeeded = true;
			}
			@Override
			public void xPressed() {
				if(3==criter.size())
					return;
				this.cg.components.subList(this.cg.cursorIndex, this.cg.cursorIndex+2).clear();
				criter.subList(this.cg.cursorIndex, this.cg.cursorIndex+2).clear();
				this.cg.context.redrawNeeded = true;
			}
			@Override
			public void spacePressed() {
				this.cg.modalD = new ComponentGroupCanBeClosedWithEsc(this.cg);
				this.cg.modalD.components.add(new ButtonWithFixedText(this.cg.modalD, "Add criterion"){
					@Override
					public void spacePressed() {
						PrimaryStruct st = criter.get(this.cg.cg.cursorIndex);
						criter.add(this.cg.cg.cursorIndex, PrimaryStruct.make("and"));
						this.cg.cg.components.add(this.cg.cg.cursorIndex, makeAnd("and"));
						if(st.getl()==0L)
							criter.add(this.cg.cg.cursorIndex, PrimaryStruct.make(st.gets()));
						else
							criter.add(this.cg.cg.cursorIndex, PrimaryStruct.make(st.getl(),st.gets()));
						this.cg.cg.components.add(this.cg.cg.cursorIndex, makeCriter(javaStr));
						this.cg.closeItself();
						this.cg.context.redrawNeeded = true;
					}
				});
				this.cg.modalD.components.add(new ButtonWithFixedText(this.cg.modalD, "Move down"){
					@Override
					public void spacePressed() {
						if(this.cg.cg.cursorIndex+2==criter.size())
							return;
						Collections.swap(this.cg.cg.components, this.cg.cg.cursorIndex, this.cg.cg.cursorIndex+2);
						Collections.swap(criter, this.cg.cg.cursorIndex, this.cg.cg.cursorIndex+2);
						this.cg.closeItself();
						this.cg.context.redrawNeeded = true;
					}
				});
				this.cg.modalD.components.add(new ButtonWithFixedText(this.cg.modalD, "Move up"){
					@Override
					public void spacePressed() {
						if(this.cg.cg.cursorIndex==1)
							return;
						Collections.swap(this.cg.cg.components, this.cg.cg.cursorIndex, this.cg.cg.cursorIndex-2);
						Collections.swap(criter, this.cg.cg.cursorIndex, this.cg.cg.cursorIndex-2);
						this.cg.closeItself();
						this.cg.context.redrawNeeded = true;
					}
				});
				this.cg.context.redrawNeeded = true;
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
				fillEditorWithLines(tyname+"Url bookmark advanced search page in new tab\n");
			}
			@Override
			public void spacePressed() {
				this.cg.context.editor.addTabNSwitchToIt(new UrlBookmarkAdvSearch(this.cg.context));
			}
		};
	}
	@Override
	public String toString() {
		return "Url Bookmark Advanced Search";
	}
}
