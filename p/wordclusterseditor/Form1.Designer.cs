namespace wordclusterseditor
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.wordListLiBo = new System.Windows.Forms.ListBox();
            this.wordListSeekLiBo = new System.Windows.Forms.ListBox();
            this.wordListAddBu = new System.Windows.Forms.Button();
            this.wordListSeekAddBu = new System.Windows.Forms.Button();
            this.wordListSeekTeBo = new System.Windows.Forms.TextBox();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.wordClustersLiBo = new System.Windows.Forms.ListBox();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.wordClustersSeek2TeBo = new System.Windows.Forms.TextBox();
            this.wordClustersSeekTeBo = new System.Windows.Forms.TextBox();
            this.wordClustersSeekLiBo = new System.Windows.Forms.ListBox();
            this.wordListSeekABu = new System.Windows.Forms.Button();
            this.wordListSeekPBu = new System.Windows.Forms.Button();
            this.proposedClustersLiBo = new System.Windows.Forms.ListBox();
            this.wordClustersAddBu = new System.Windows.Forms.Button();
            this.proposedWordsLiBo = new System.Windows.Forms.ListBox();
            this.apposeBu = new System.Windows.Forms.Button();
            this.wordsRemoveBu = new System.Windows.Forms.Button();
            this.wordClustersRemoveBu = new System.Windows.Forms.Button();
            this.editingAClusterLiBo = new System.Windows.Forms.ListBox();
            this.expungeBu = new System.Windows.Forms.Button();
            this.aNewClusterBu = new System.Windows.Forms.Button();
            this.confirmBu = new System.Windows.Forms.Button();
            this.aNewClusterTeBo = new System.Windows.Forms.TextBox();
            this.aNewWordTeBo = new System.Windows.Forms.TextBox();
            this.aNewWordBu = new System.Windows.Forms.Button();
            this.clearClustersBu = new System.Windows.Forms.Button();
            this.clearWordsBu = new System.Windows.Forms.Button();
            this.extantClustersLiBo = new System.Windows.Forms.ListBox();
            this.aClusterLiBo = new System.Windows.Forms.ListBox();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.SuspendLayout();
            // 
            // wordListLiBo
            // 
            this.wordListLiBo.Font = new System.Drawing.Font("Verdana", 8.25F);
            this.wordListLiBo.FormattingEnabled = true;
            this.wordListLiBo.Location = new System.Drawing.Point(0, 39);
            this.wordListLiBo.Name = "wordListLiBo";
            this.wordListLiBo.Size = new System.Drawing.Size(120, 550);
            this.wordListLiBo.TabIndex = 0;
            this.wordListLiBo.SelectedIndexChanged += new System.EventHandler(this.wordListLiBo_SelectedIndexChanged);
            this.wordListLiBo.KeyDown += new System.Windows.Forms.KeyEventHandler(this.wordListLiBo_KeyDown);
            // 
            // wordListSeekLiBo
            // 
            this.wordListSeekLiBo.Font = new System.Drawing.Font("Verdana", 8.25F);
            this.wordListSeekLiBo.FormattingEnabled = true;
            this.wordListSeekLiBo.Location = new System.Drawing.Point(126, 91);
            this.wordListSeekLiBo.Name = "wordListSeekLiBo";
            this.wordListSeekLiBo.Size = new System.Drawing.Size(120, 498);
            this.wordListSeekLiBo.Sorted = true;
            this.wordListSeekLiBo.TabIndex = 1;
            this.wordListSeekLiBo.SelectedIndexChanged += new System.EventHandler(this.wordListSeekLiBo_SelectedIndexChanged);
            this.wordListSeekLiBo.KeyDown += new System.Windows.Forms.KeyEventHandler(this.wordListSeekLiBo_KeyDown);
            // 
            // wordListAddBu
            // 
            this.wordListAddBu.Location = new System.Drawing.Point(0, 594);
            this.wordListAddBu.Name = "wordListAddBu";
            this.wordListAddBu.Size = new System.Drawing.Size(120, 70);
            this.wordListAddBu.TabIndex = 2;
            this.wordListAddBu.Text = "ADD";
            this.wordListAddBu.UseVisualStyleBackColor = true;
            this.wordListAddBu.Click += new System.EventHandler(this.wordListAddBu_Click);
            // 
            // wordListSeekAddBu
            // 
            this.wordListSeekAddBu.Location = new System.Drawing.Point(126, 594);
            this.wordListSeekAddBu.Name = "wordListSeekAddBu";
            this.wordListSeekAddBu.Size = new System.Drawing.Size(120, 70);
            this.wordListSeekAddBu.TabIndex = 0;
            this.wordListSeekAddBu.Text = "ADD";
            this.wordListSeekAddBu.UseVisualStyleBackColor = true;
            this.wordListSeekAddBu.Click += new System.EventHandler(this.wordListSeekAddBu_Click);
            // 
            // wordListSeekTeBo
            // 
            this.wordListSeekTeBo.Font = new System.Drawing.Font("Verdana", 8.25F);
            this.wordListSeekTeBo.Location = new System.Drawing.Point(0, 12);
            this.wordListSeekTeBo.Name = "wordListSeekTeBo";
            this.wordListSeekTeBo.Size = new System.Drawing.Size(246, 21);
            this.wordListSeekTeBo.TabIndex = 3;
            this.wordListSeekTeBo.KeyDown += new System.Windows.Forms.KeyEventHandler(this.wordListSeekTeBo_KeyDown);
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Location = new System.Drawing.Point(252, 26);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(390, 316);
            this.tabControl1.TabIndex = 4;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.wordClustersLiBo);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(382, 290);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "FUL";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // wordClustersLiBo
            // 
            this.wordClustersLiBo.Font = new System.Drawing.Font("Verdana", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.wordClustersLiBo.FormattingEnabled = true;
            this.wordClustersLiBo.Location = new System.Drawing.Point(0, 0);
            this.wordClustersLiBo.Name = "wordClustersLiBo";
            this.wordClustersLiBo.Size = new System.Drawing.Size(382, 290);
            this.wordClustersLiBo.Sorted = true;
            this.wordClustersLiBo.TabIndex = 0;
            this.wordClustersLiBo.SelectedIndexChanged += new System.EventHandler(this.wordClustersLiBo_SelectedIndexChanged);
            this.wordClustersLiBo.KeyDown += new System.Windows.Forms.KeyEventHandler(this.wordClustersLiBo_KeyDown);
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.label2);
            this.tabPage2.Controls.Add(this.label1);
            this.tabPage2.Controls.Add(this.wordClustersSeek2TeBo);
            this.tabPage2.Controls.Add(this.wordClustersSeekTeBo);
            this.tabPage2.Controls.Add(this.wordClustersSeekLiBo);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(382, 290);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "SEE";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(295, 267);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(42, 13);
            this.label2.TabIndex = 6;
            this.label2.Text = "WORD";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(295, 241);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(48, 13);
            this.label1.TabIndex = 5;
            this.label1.Text = "STRING";
            // 
            // wordClustersSeek2TeBo
            // 
            this.wordClustersSeek2TeBo.Font = new System.Drawing.Font("Verdana", 8.25F);
            this.wordClustersSeek2TeBo.Location = new System.Drawing.Point(0, 238);
            this.wordClustersSeek2TeBo.Name = "wordClustersSeek2TeBo";
            this.wordClustersSeek2TeBo.Size = new System.Drawing.Size(289, 21);
            this.wordClustersSeek2TeBo.TabIndex = 4;
            this.wordClustersSeek2TeBo.KeyDown += new System.Windows.Forms.KeyEventHandler(this.wordClustersSeek2TeBo_KeyDown);
            // 
            // wordClustersSeekTeBo
            // 
            this.wordClustersSeekTeBo.Font = new System.Drawing.Font("Verdana", 8.25F);
            this.wordClustersSeekTeBo.Location = new System.Drawing.Point(0, 264);
            this.wordClustersSeekTeBo.Name = "wordClustersSeekTeBo";
            this.wordClustersSeekTeBo.Size = new System.Drawing.Size(289, 21);
            this.wordClustersSeekTeBo.TabIndex = 3;
            this.wordClustersSeekTeBo.KeyDown += new System.Windows.Forms.KeyEventHandler(this.wordClustersSeekTeBo_KeyDown);
            // 
            // wordClustersSeekLiBo
            // 
            this.wordClustersSeekLiBo.Font = new System.Drawing.Font("Verdana", 8.25F);
            this.wordClustersSeekLiBo.FormattingEnabled = true;
            this.wordClustersSeekLiBo.Location = new System.Drawing.Point(0, 0);
            this.wordClustersSeekLiBo.Name = "wordClustersSeekLiBo";
            this.wordClustersSeekLiBo.Size = new System.Drawing.Size(382, 225);
            this.wordClustersSeekLiBo.Sorted = true;
            this.wordClustersSeekLiBo.TabIndex = 0;
            this.wordClustersSeekLiBo.SelectedIndexChanged += new System.EventHandler(this.wordClustersSeekLiBo_SelectedIndexChanged);
            this.wordClustersSeekLiBo.KeyDown += new System.Windows.Forms.KeyEventHandler(this.wordClustersSeekLiBo_KeyDown);
            // 
            // wordListSeekABu
            // 
            this.wordListSeekABu.Location = new System.Drawing.Point(126, 38);
            this.wordListSeekABu.Name = "wordListSeekABu";
            this.wordListSeekABu.Size = new System.Drawing.Size(120, 23);
            this.wordListSeekABu.TabIndex = 2;
            this.wordListSeekABu.Text = "ANTERIOR";
            this.wordListSeekABu.UseVisualStyleBackColor = true;
            this.wordListSeekABu.Click += new System.EventHandler(this.wordListSeekABu_Click);
            // 
            // wordListSeekPBu
            // 
            this.wordListSeekPBu.Location = new System.Drawing.Point(126, 62);
            this.wordListSeekPBu.Name = "wordListSeekPBu";
            this.wordListSeekPBu.Size = new System.Drawing.Size(120, 23);
            this.wordListSeekPBu.TabIndex = 1;
            this.wordListSeekPBu.Text = "POSTERIOR";
            this.wordListSeekPBu.UseVisualStyleBackColor = true;
            this.wordListSeekPBu.Click += new System.EventHandler(this.wordListSeekPBu_Click);
            // 
            // proposedClustersLiBo
            // 
            this.proposedClustersLiBo.Font = new System.Drawing.Font("Verdana", 8.25F);
            this.proposedClustersLiBo.FormattingEnabled = true;
            this.proposedClustersLiBo.Location = new System.Drawing.Point(252, 384);
            this.proposedClustersLiBo.Name = "proposedClustersLiBo";
            this.proposedClustersLiBo.Size = new System.Drawing.Size(386, 251);
            this.proposedClustersLiBo.Sorted = true;
            this.proposedClustersLiBo.TabIndex = 5;
            this.proposedClustersLiBo.SelectedIndexChanged += new System.EventHandler(this.proposedClustersLiBo_SelectedIndexChanged);
            // 
            // wordClustersAddBu
            // 
            this.wordClustersAddBu.Location = new System.Drawing.Point(370, 0);
            this.wordClustersAddBu.Name = "wordClustersAddBu";
            this.wordClustersAddBu.Size = new System.Drawing.Size(268, 42);
            this.wordClustersAddBu.TabIndex = 6;
            this.wordClustersAddBu.Text = "ADD";
            this.wordClustersAddBu.UseVisualStyleBackColor = true;
            this.wordClustersAddBu.Click += new System.EventHandler(this.wordClustersAddBu_Click);
            // 
            // proposedWordsLiBo
            // 
            this.proposedWordsLiBo.Font = new System.Drawing.Font("Verdana", 8.25F);
            this.proposedWordsLiBo.FormattingEnabled = true;
            this.proposedWordsLiBo.Location = new System.Drawing.Point(648, 384);
            this.proposedWordsLiBo.Name = "proposedWordsLiBo";
            this.proposedWordsLiBo.Size = new System.Drawing.Size(386, 251);
            this.proposedWordsLiBo.Sorted = true;
            this.proposedWordsLiBo.TabIndex = 7;
            this.proposedWordsLiBo.SelectedIndexChanged += new System.EventHandler(this.proposedWordsLiBo_SelectedIndexChanged);
            this.proposedWordsLiBo.KeyDown += new System.Windows.Forms.KeyEventHandler(this.proposedWordsLiBo_KeyDown);
            // 
            // apposeBu
            // 
            this.apposeBu.Location = new System.Drawing.Point(1040, 345);
            this.apposeBu.Name = "apposeBu";
            this.apposeBu.Size = new System.Drawing.Size(240, 36);
            this.apposeBu.TabIndex = 8;
            this.apposeBu.Text = "APPOSE";
            this.apposeBu.UseVisualStyleBackColor = true;
            this.apposeBu.Click += new System.EventHandler(this.apposeBu_Click);
            // 
            // wordsRemoveBu
            // 
            this.wordsRemoveBu.Location = new System.Drawing.Point(937, 641);
            this.wordsRemoveBu.Name = "wordsRemoveBu";
            this.wordsRemoveBu.Size = new System.Drawing.Size(97, 23);
            this.wordsRemoveBu.TabIndex = 9;
            this.wordsRemoveBu.Text = "REM";
            this.wordsRemoveBu.UseVisualStyleBackColor = true;
            this.wordsRemoveBu.Click += new System.EventHandler(this.wordsRemoveBu_Click);
            // 
            // wordClustersRemoveBu
            // 
            this.wordClustersRemoveBu.Location = new System.Drawing.Point(541, 641);
            this.wordClustersRemoveBu.Name = "wordClustersRemoveBu";
            this.wordClustersRemoveBu.Size = new System.Drawing.Size(97, 23);
            this.wordClustersRemoveBu.TabIndex = 10;
            this.wordClustersRemoveBu.Text = "REM";
            this.wordClustersRemoveBu.UseVisualStyleBackColor = true;
            this.wordClustersRemoveBu.Click += new System.EventHandler(this.wordClustersRemoveBu_Click);
            // 
            // editingAClusterLiBo
            // 
            this.editingAClusterLiBo.Font = new System.Drawing.Font("Verdana", 8.25F);
            this.editingAClusterLiBo.FormattingEnabled = true;
            this.editingAClusterLiBo.Location = new System.Drawing.Point(648, 0);
            this.editingAClusterLiBo.Name = "editingAClusterLiBo";
            this.editingAClusterLiBo.Size = new System.Drawing.Size(120, 342);
            this.editingAClusterLiBo.Sorted = true;
            this.editingAClusterLiBo.TabIndex = 11;
            this.editingAClusterLiBo.SelectedIndexChanged += new System.EventHandler(this.editingAClusterLiBo_SelectedIndexChanged);
            // 
            // expungeBu
            // 
            this.expungeBu.Location = new System.Drawing.Point(774, 56);
            this.expungeBu.Name = "expungeBu";
            this.expungeBu.Size = new System.Drawing.Size(75, 50);
            this.expungeBu.TabIndex = 12;
            this.expungeBu.Text = "EXPUNGE";
            this.expungeBu.UseVisualStyleBackColor = true;
            this.expungeBu.Click += new System.EventHandler(this.expungeBu_Click);
            // 
            // aNewClusterBu
            // 
            this.aNewClusterBu.Location = new System.Drawing.Point(252, 641);
            this.aNewClusterBu.Name = "aNewClusterBu";
            this.aNewClusterBu.Size = new System.Drawing.Size(75, 23);
            this.aNewClusterBu.TabIndex = 13;
            this.aNewClusterBu.Text = "A New Clr";
            this.aNewClusterBu.UseVisualStyleBackColor = true;
            this.aNewClusterBu.Click += new System.EventHandler(this.aNewClusterBu_Click);
            // 
            // confirmBu
            // 
            this.confirmBu.Enabled = false;
            this.confirmBu.Location = new System.Drawing.Point(774, 0);
            this.confirmBu.Name = "confirmBu";
            this.confirmBu.Size = new System.Drawing.Size(75, 50);
            this.confirmBu.TabIndex = 14;
            this.confirmBu.Text = "CONFIRM";
            this.confirmBu.UseVisualStyleBackColor = true;
            this.confirmBu.Click += new System.EventHandler(this.confirmBu_Click);
            // 
            // aNewClusterTeBo
            // 
            this.aNewClusterTeBo.Font = new System.Drawing.Font("Verdana", 8.25F);
            this.aNewClusterTeBo.Location = new System.Drawing.Point(333, 643);
            this.aNewClusterTeBo.Name = "aNewClusterTeBo";
            this.aNewClusterTeBo.Size = new System.Drawing.Size(202, 21);
            this.aNewClusterTeBo.TabIndex = 15;
            // 
            // aNewWordTeBo
            // 
            this.aNewWordTeBo.Font = new System.Drawing.Font("Verdana", 8.25F);
            this.aNewWordTeBo.Location = new System.Drawing.Point(729, 643);
            this.aNewWordTeBo.Name = "aNewWordTeBo";
            this.aNewWordTeBo.Size = new System.Drawing.Size(202, 21);
            this.aNewWordTeBo.TabIndex = 5;
            this.aNewWordTeBo.KeyDown += new System.Windows.Forms.KeyEventHandler(this.aNewWordTeBo_KeyDown);
            // 
            // aNewWordBu
            // 
            this.aNewWordBu.Location = new System.Drawing.Point(648, 641);
            this.aNewWordBu.Name = "aNewWordBu";
            this.aNewWordBu.Size = new System.Drawing.Size(75, 23);
            this.aNewWordBu.TabIndex = 16;
            this.aNewWordBu.Text = "A Temp Wrd";
            this.aNewWordBu.UseVisualStyleBackColor = true;
            this.aNewWordBu.Click += new System.EventHandler(this.aNewWordBu_Click);
            // 
            // clearClustersBu
            // 
            this.clearClustersBu.Location = new System.Drawing.Point(252, 345);
            this.clearClustersBu.Name = "clearClustersBu";
            this.clearClustersBu.Size = new System.Drawing.Size(386, 36);
            this.clearClustersBu.TabIndex = 1;
            this.clearClustersBu.Text = "CLEAR";
            this.clearClustersBu.UseVisualStyleBackColor = true;
            this.clearClustersBu.Click += new System.EventHandler(this.clearClustersBu_Click);
            // 
            // clearWordsBu
            // 
            this.clearWordsBu.Location = new System.Drawing.Point(648, 345);
            this.clearWordsBu.Name = "clearWordsBu";
            this.clearWordsBu.Size = new System.Drawing.Size(386, 36);
            this.clearWordsBu.TabIndex = 0;
            this.clearWordsBu.Text = "CLEAR";
            this.clearWordsBu.UseVisualStyleBackColor = true;
            this.clearWordsBu.Click += new System.EventHandler(this.clearWordsBu_Click);
            // 
            // extantClustersLiBo
            // 
            this.extantClustersLiBo.Font = new System.Drawing.Font("Verdana", 8.25F);
            this.extantClustersLiBo.FormattingEnabled = true;
            this.extantClustersLiBo.Location = new System.Drawing.Point(1040, 384);
            this.extantClustersLiBo.Name = "extantClustersLiBo";
            this.extantClustersLiBo.Size = new System.Drawing.Size(240, 277);
            this.extantClustersLiBo.TabIndex = 17;
            // 
            // aClusterLiBo
            // 
            this.aClusterLiBo.Font = new System.Drawing.Font("Verdana", 8.25F);
            this.aClusterLiBo.FormattingEnabled = true;
            this.aClusterLiBo.Location = new System.Drawing.Point(855, 0);
            this.aClusterLiBo.Name = "aClusterLiBo";
            this.aClusterLiBo.Size = new System.Drawing.Size(120, 342);
            this.aClusterLiBo.TabIndex = 18;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1292, 666);
            this.Controls.Add(this.aClusterLiBo);
            this.Controls.Add(this.extantClustersLiBo);
            this.Controls.Add(this.clearWordsBu);
            this.Controls.Add(this.clearClustersBu);
            this.Controls.Add(this.wordListSeekPBu);
            this.Controls.Add(this.wordListSeekABu);
            this.Controls.Add(this.aNewWordBu);
            this.Controls.Add(this.aNewWordTeBo);
            this.Controls.Add(this.aNewClusterTeBo);
            this.Controls.Add(this.confirmBu);
            this.Controls.Add(this.aNewClusterBu);
            this.Controls.Add(this.expungeBu);
            this.Controls.Add(this.editingAClusterLiBo);
            this.Controls.Add(this.wordClustersRemoveBu);
            this.Controls.Add(this.wordsRemoveBu);
            this.Controls.Add(this.apposeBu);
            this.Controls.Add(this.proposedWordsLiBo);
            this.Controls.Add(this.wordClustersAddBu);
            this.Controls.Add(this.proposedClustersLiBo);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.wordListSeekTeBo);
            this.Controls.Add(this.wordListSeekAddBu);
            this.Controls.Add(this.wordListAddBu);
            this.Controls.Add(this.wordListSeekLiBo);
            this.Controls.Add(this.wordListLiBo);
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Form1";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListBox wordListLiBo;
        private System.Windows.Forms.ListBox wordListSeekLiBo;
        private System.Windows.Forms.Button wordListAddBu;
        private System.Windows.Forms.Button wordListSeekAddBu;
        private System.Windows.Forms.TextBox wordListSeekTeBo;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.ListBox wordClustersLiBo;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.ListBox wordClustersSeekLiBo;
        private System.Windows.Forms.ListBox proposedClustersLiBo;
        private System.Windows.Forms.Button wordClustersAddBu;
        private System.Windows.Forms.TextBox wordClustersSeekTeBo;
        private System.Windows.Forms.Button wordListSeekABu;
        private System.Windows.Forms.Button wordListSeekPBu;
        private System.Windows.Forms.ListBox proposedWordsLiBo;
        private System.Windows.Forms.Button apposeBu;
        private System.Windows.Forms.Button wordsRemoveBu;
        private System.Windows.Forms.Button wordClustersRemoveBu;
        private System.Windows.Forms.ListBox editingAClusterLiBo;
        private System.Windows.Forms.Button expungeBu;
        private System.Windows.Forms.Button aNewClusterBu;
        private System.Windows.Forms.Button confirmBu;
        private System.Windows.Forms.TextBox aNewClusterTeBo;
        private System.Windows.Forms.TextBox wordClustersSeek2TeBo;
        private System.Windows.Forms.TextBox aNewWordTeBo;
        private System.Windows.Forms.Button aNewWordBu;
        private System.Windows.Forms.Button clearClustersBu;
        private System.Windows.Forms.Button clearWordsBu;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ListBox extantClustersLiBo;
        private System.Windows.Forms.ListBox aClusterLiBo;
    }
}

