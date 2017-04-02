using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace wordclusterseditor
{
    public partial class Form1 : Form
    {
        DialogResult result1DNe;
        string wordsString, wordClustersString, editingString="";
        string[] wordsStrings, wordClustersStrings;
        string[] splittingStrings={"\r\n"};
        string[] editingCopyStrings;

        public Form1()
        {
            InitializeComponent();
            StreamReader streamReader1 = new StreamReader("lexicon.txt");
            wordsString = streamReader1.ReadToEnd();
            streamReader1.Close();
            streamReader1 = new StreamReader("wordclusters.txt");
            wordClustersString = streamReader1.ReadToEnd();
            streamReader1.Close();
            wordsStrings = wordsString.Split(splittingStrings, StringSplitOptions.RemoveEmptyEntries);
            foreach (string s in wordsStrings)
                wordListLiBo.Items.Add(s);
            wordClustersStrings = wordClustersString.Split(splittingStrings, StringSplitOptions.RemoveEmptyEntries);
            foreach (string s in wordClustersStrings)
                wordClustersLiBo.Items.Add(s);
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            result1DNe = MessageBox.Show("Save it?", "WCE", MessageBoxButtons.YesNo);
            if (result1DNe == DialogResult.Yes)
            {
                StreamWriter streamWriter1 = new StreamWriter("wordclusters.txt");
                foreach (System.Object editingOb in wordClustersLiBo.Items)
                {
                    if (editingString != "")
                        editingString += "\r\n";
                    editingString += editingOb.ToString();
                }
                streamWriter1.Write(editingString);
                streamWriter1.Close();
            }
        }

        private void expungeBu_Click(object sender, EventArgs e)
        {
            if (editingAClusterLiBo.SelectedItem == null)
                return;
            editingAClusterLiBo.Items.Remove(editingAClusterLiBo.SelectedItem);
            foreach (System.Object editingOb in editingAClusterLiBo.Items)
            {
                if (editingString != "")
                    editingString += ' ';
                editingString += editingOb.ToString();
            }
            if (editingString != proposedClustersLiBo.SelectedItem.ToString())
                confirmBu.Enabled = true;
            else
                confirmBu.Enabled = false;
            editingString = "";
        }

        private void wordClustersAddBu_Click(object sender, EventArgs e)
        {
            if (tabControl1.SelectedIndex == 0)
            {
                if (wordClustersLiBo.SelectedItem == null)
                    return;
                foreach (System.Object editingOb in proposedClustersLiBo.Items)
                    if (wordClustersLiBo.SelectedItem == editingOb) return;
                proposedClustersLiBo.Items.Add(wordClustersLiBo.SelectedItem);
            }
            else
            {
                if (wordClustersSeekLiBo.SelectedItem == null)
                    return;
                foreach (System.Object editingOb in proposedClustersLiBo.Items)
                    if (wordClustersSeekLiBo.SelectedItem == editingOb) return;
                proposedClustersLiBo.Items.Add(wordClustersSeekLiBo.SelectedItem);
            }
        }

        private void wordListAddBu_Click(object sender, EventArgs e)
        {
            if (wordListLiBo.SelectedItem == null)
                return;
            foreach (System.Object editingOb in proposedWordsLiBo.Items)
                if (wordListLiBo.SelectedItem == editingOb) return;
            proposedWordsLiBo.Items.Add(wordListLiBo.SelectedItem);
        }

        private void wordListSeekAddBu_Click(object sender, EventArgs e)
        {
            if (wordListSeekLiBo.SelectedItem == null)
                return;
            foreach (System.Object editingOb in proposedWordsLiBo.Items)
                if (wordListSeekLiBo.SelectedItem == editingOb) return;
            proposedWordsLiBo.Items.Add(wordListSeekLiBo.SelectedItem);
        }

        private void apposeBu_Click(object sender, EventArgs e)
        {
            if (proposedWordsLiBo.Items.Count == 0)
                return;
            editingAClusterLiBo.Items.Clear();
            confirmBu.Enabled = false;
            foreach (System.Object editingOb in proposedWordsLiBo.Items)
            {
                if (editingString != "")
                    editingString += ' ';
                editingString += editingOb.ToString();
            }
            wordClustersLiBo.Items.Add(editingString);
            proposedClustersLiBo.Items.Add(editingString);
            proposedClustersLiBo.SelectedItem = editingString;
            editingString = "";
        }

        private void proposedClustersLiBo_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (proposedClustersLiBo.SelectedItem == null)
                return;
            editingAClusterLiBo.Items.Clear();
            editingCopyStrings = proposedClustersLiBo.SelectedItem.ToString().Split(' ');
            editingAClusterLiBo.Items.AddRange(editingCopyStrings);
            Clipboard.SetText(proposedClustersLiBo.SelectedItem.ToString());
        }

        private void confirmBu_Click(object sender, EventArgs e)
        {
            confirmBu.Enabled=false;
            wordClustersLiBo.Items.Remove(proposedClustersLiBo.SelectedItem);
            wordClustersSeekLiBo.Items.Remove(proposedClustersLiBo.SelectedItem);
            foreach (System.Object editingOb in editingAClusterLiBo.Items)
            {
                if (editingString != "")
                    editingString += ' ';
                editingString += editingOb.ToString();
            }
            wordClustersLiBo.Items.Add(editingString);
            proposedClustersLiBo.Items.Add(editingString);
            proposedClustersLiBo.Items.Remove(proposedClustersLiBo.SelectedItem);
            proposedClustersLiBo.SelectedItem = editingString;
            editingString = "";
        }

        private void aNewClusterBu_Click(object sender, EventArgs e)
        {
            if (aNewClusterTeBo.Text == "")
                return;
            wordClustersLiBo.Items.Add(aNewClusterTeBo.Text);
            proposedClustersLiBo.Items.Add(aNewClusterTeBo.Text);
            proposedClustersLiBo.SelectedItem = aNewClusterTeBo.Text;
            aNewClusterTeBo.Text = "";
        }

        private void wordsRemoveBu_Click(object sender, EventArgs e)
        {
            proposedWordsLiBo.Items.Remove(proposedWordsLiBo.SelectedItem);
        }

        private void wordClustersRemoveBu_Click(object sender, EventArgs e)
        {
            proposedClustersLiBo.Items.Remove(proposedClustersLiBo.SelectedItem);
            editingAClusterLiBo.Items.Clear();
        }

        private void wordListSeekTeBo_KeyDown(object sender, KeyEventArgs e)
        {
            if(wordListSeekTeBo.Text=="")
                return;
            if (e.KeyCode == Keys.Enter)
            {
                wordListSeekLiBo.Items.Clear();
                foreach (System.Object editingOb in wordListLiBo.Items)
                    if (editingOb.ToString().ToLower().IndexOf(wordListSeekTeBo.Text) != -1) wordListSeekLiBo.Items.Add(editingOb);
            }
        }

        private void wordClustersSeek2TeBo_KeyDown(object sender, KeyEventArgs e)
        {
            if (wordClustersSeek2TeBo.Text == "")
                return;
            if (e.KeyCode == Keys.Enter)
            {
                wordClustersSeekLiBo.Items.Clear();
                foreach (System.Object editingOb in wordClustersLiBo.Items)
                    if (editingOb.ToString().ToLower().IndexOf(wordClustersSeek2TeBo.Text) != -1) wordClustersSeekLiBo.Items.Add(editingOb);
            }
        }

        private void wordClustersSeekTeBo_KeyDown(object sender, KeyEventArgs e)
        {
            if (wordClustersSeekTeBo.Text == "")
                return;
            if (e.KeyCode == Keys.Enter)
            {
                wordClustersSeekLiBo.Items.Clear();
                foreach (System.Object editingOb in wordClustersLiBo.Items)
                    foreach (string s in editingOb.ToString().ToLower().Split(' '))
                        if (s == wordClustersSeekTeBo.Text)
                        {
                            wordClustersSeekLiBo.Items.Add(editingOb);
                            break;
                        }
            }
        }

        private void aNewWordBu_Click(object sender, EventArgs e)
        {
            if (aNewWordTeBo.Text == "")
                return;
            proposedWordsLiBo.Items.Add(aNewWordTeBo.Text);
            proposedWordsLiBo.SelectedItem = aNewWordTeBo.Text;
        }

        private void wordListLiBo_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (wordListLiBo.SelectedItem == null)
                return;
            Clipboard.SetText(wordListLiBo.SelectedItem.ToString());
        }

        private void wordListSeekLiBo_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (wordListSeekLiBo.SelectedItem == null)
                return;
            Clipboard.SetText(wordListSeekLiBo.SelectedItem.ToString());
        }

        private void proposedWordsLiBo_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (proposedWordsLiBo.SelectedItem == null)
                return;
            Clipboard.SetText(proposedWordsLiBo.SelectedItem.ToString());
            extantClustersLiBo.Items.Clear();
            foreach (System.Object editingOb in wordClustersLiBo.Items)
                foreach (string s in editingOb.ToString().ToLower().Split(' '))
                    if (s == proposedWordsLiBo.SelectedItem.ToString())
                    {
                        extantClustersLiBo.Items.Add(editingOb);
                        break;
                    }
        }

        private void editingAClusterLiBo_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (editingAClusterLiBo.SelectedItem == null)
                return;
            Clipboard.SetText(editingAClusterLiBo.SelectedItem.ToString());
        }

        private void wordClustersSeekLiBo_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (wordClustersSeekLiBo.SelectedItem == null)
                return;
            aClusterLiBo.Items.Clear();
            editingCopyStrings = wordClustersSeekLiBo.SelectedItem.ToString().Split(' ');
            aClusterLiBo.Items.AddRange(editingCopyStrings);
            Clipboard.SetText(wordClustersSeekLiBo.SelectedItem.ToString());
        }

        private void wordClustersLiBo_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (wordClustersLiBo.SelectedItem == null)
                return;
            aClusterLiBo.Items.Clear();
            editingCopyStrings = wordClustersLiBo.SelectedItem.ToString().Split(' ');
            aClusterLiBo.Items.AddRange(editingCopyStrings);
            Clipboard.SetText(wordClustersLiBo.SelectedItem.ToString());
        }

        private void wordListLiBo_KeyDown(object sender, KeyEventArgs e)
        {
            if (wordListLiBo.SelectedItem == null)
                return;
            if (e.KeyCode != Keys.Enter)
                return;
            foreach (System.Object editingOb in proposedWordsLiBo.Items)
                if (wordListLiBo.SelectedItem == editingOb) return;
            proposedWordsLiBo.Items.Add(wordListLiBo.SelectedItem);
        }

        private void wordClustersSeekLiBo_KeyDown(object sender, KeyEventArgs e)
        {
            if (wordClustersSeekLiBo.SelectedItem == null)
                return;
            if (e.KeyCode != Keys.Enter)
                return;
            foreach (System.Object editingOb in proposedClustersLiBo.Items)
                if (wordClustersSeekLiBo.SelectedItem == editingOb) return;
            proposedClustersLiBo.Items.Add(wordClustersSeekLiBo.SelectedItem);
        }

        private void wordListSeekLiBo_KeyDown(object sender, KeyEventArgs e)
        {
            if (wordListSeekLiBo.SelectedItem == null)
                return;
            if (e.KeyCode != Keys.Enter)
                return;
            foreach (System.Object editingOb in proposedWordsLiBo.Items)
                if (wordListSeekLiBo.SelectedItem == editingOb) return;
            proposedWordsLiBo.Items.Add(wordListSeekLiBo.SelectedItem);
        }

        private void wordClustersLiBo_KeyDown(object sender, KeyEventArgs e)
        {
            if (wordClustersLiBo.SelectedItem == null)
                return;
            if (e.KeyCode != Keys.Enter)
                return;
            foreach (System.Object editingOb in proposedClustersLiBo.Items)
                if (wordClustersLiBo.SelectedItem == editingOb) return;
            proposedClustersLiBo.Items.Add(wordClustersLiBo.SelectedItem);
        }

        private void clearClustersBu_Click(object sender, EventArgs e)
        {
            proposedClustersLiBo.Items.Clear();
            editingAClusterLiBo.Items.Clear();
            confirmBu.Enabled = false;
        }

        private void clearWordsBu_Click(object sender, EventArgs e)
        {
            proposedWordsLiBo.Items.Clear();
        }

        private void proposedWordsLiBo_KeyDown(object sender, KeyEventArgs e)
        {
            if (proposedClustersLiBo.SelectedItem == null)
                return;
            if (e.KeyCode != Keys.Enter)
                return;
            if (proposedWordsLiBo.SelectedItem == null)
                return;
            foreach (System.Object editingOb in editingAClusterLiBo.Items)
                if (proposedWordsLiBo.SelectedItem.ToString() == editingOb.ToString()) return;
            editingAClusterLiBo.Items.Add(proposedWordsLiBo.SelectedItem);
            foreach (System.Object editingOb in editingAClusterLiBo.Items)
            {
                if (editingString != "")
                    editingString += ' ';
                editingString += editingOb.ToString();
            }
            if (editingString != proposedClustersLiBo.SelectedItem.ToString())
                confirmBu.Enabled = true;
            else
                confirmBu.Enabled = false;
            editingString = "";
        }

        private void wordListSeekABu_Click(object sender, EventArgs e)
        {
            if (wordListSeekTeBo.Text == "")
                return;
            wordListSeekLiBo.Items.Clear();
            foreach (System.Object editingOb in wordListLiBo.Items)
                if (editingOb.ToString().ToLower().IndexOf(wordListSeekTeBo.Text) == 0) wordListSeekLiBo.Items.Add(editingOb);
        }

        private void wordListSeekPBu_Click(object sender, EventArgs e)
        {
            if (wordListSeekTeBo.Text == "")
                return;
            wordListSeekLiBo.Items.Clear();
            int int1;
            string seekPString = wordListSeekTeBo.Text;
            foreach (System.Object editingOb in wordListLiBo.Items)
            {
                int1 = editingOb.ToString().ToLower().LastIndexOf(seekPString);
                if (int1 + seekPString.Length == editingOb.ToString().Length && int1 != -1) wordListSeekLiBo.Items.Add(editingOb);
            }
        }

        private void aNewWordTeBo_KeyDown(object sender, KeyEventArgs e)
        {
            if (aNewWordTeBo.Text == "")
                return;
            if (e.KeyCode != Keys.Enter)
                return;
            proposedWordsLiBo.Items.Add(aNewWordTeBo.Text);
            proposedWordsLiBo.SelectedItem = aNewWordTeBo.Text;
        }
    }
}
