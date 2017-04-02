using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace WordMemorizingApp
{
    public partial class Form1 : Form
    {
        string stringWords, stringMarks, stringOTWord, stringClusters;
        string[] splittingStrings = { "\r\n" };
        string[] stringSWords, stringSEn, stringSCn, stringSClusters;
        bool boolScrambled = false, boolPrecluding = false;
        bool[] boolMarks;
        int intTally1 = 0, intTally2 = 0, intCurrentWord = 0, intR1, intR2;
        Random randomWord = new Random();
        DialogResult diReClosing;
        StreamReader streamReader1;

        public Form1()
        {
            InitializeComponent();
            streamReader1 = new StreamReader("wordlist.txt", Encoding.Unicode);
            stringWords = streamReader1.ReadToEnd();
            streamReader1.Close();
            streamReader1 = new StreamReader("marks.txt");
            stringMarks = streamReader1.ReadToEnd();
            streamReader1.Close();
            streamReader1 = new StreamReader("wordclusters.txt");
            stringClusters = streamReader1.ReadToEnd();
            streamReader1.Close();
            stringSClusters = stringClusters.Split(splittingStrings, StringSplitOptions.RemoveEmptyEntries);
            while (true)
            {
                intTally1 = stringWords.IndexOf('\n', intTally1);
                if (-1 == intTally1) break;
                intTally1++; intTally2++;
            }
            button1.Text = "lessthan" + intTally2.ToString();
            stringSEn = new string[intTally2 + 1];
            stringSCn = new string[intTally2 + 1];
            boolMarks = new bool[intTally2 + 1];
            stringSWords = stringWords.Split(splittingStrings, StringSplitOptions.RemoveEmptyEntries);
            int i = 0;
            foreach (string s in stringSWords)
            {
                stringSEn[i] = s.Remove(s.IndexOf(']') + 1);
                stringSCn[i] = s.Substring(s.IndexOf(']') + 2);
                i++;
            }
            i = 0;
            int w = 0, e = 0;
            splittingStrings = new string[] { " " };
            stringSWords = stringMarks.Split(splittingStrings, StringSplitOptions.RemoveEmptyEntries);
            foreach (string s in stringSWords)
            {
                w = Convert.ToInt32(s);
                if (boolMarks[i])
                {
                    i++;
                    for (int q = 1; q != w; q++)
                    {
                        boolMarks[i] = true;
                        i++;
                        e++;
                    }
                }
                else
                {
                    i += w;
                    boolMarks[i] = true;
                    e++;
                }
            }
            labelEn.Text = stringSEn[0];
            foreach (string s in stringSEn)
                liBoEn.Items.Add(s);
            label4.Text = e.ToString() + " / " + intTally2.ToString();
            int int1=0;
            foreach (bool b in boolMarks)
            {
                if (b) liBoCn.Items.Add(stringSCn[int1]);
                else
                    liBoCn.Items.Add("");
                int1++;
            }
        }

        private void Form1_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.W||e.KeyCode==Keys.Down)
            {
                if (textBoxCn.Text == "")
                    textBoxCn.Text = stringSCn[intCurrentWord];
                else if (intCurrentWord == intTally2)
                    intCurrentWord = 0;
                else if (boolScrambled)
                {
                    if (textBox2.Text == "" || textBox3.Text == "")
                        intCurrentWord = randomWord.Next(stringSEn.Length);
                    else
                        intCurrentWord = randomWord.Next(intR1, intR2);
                    if (boolPrecluding)
                    {
                        int int1 = 0;
                        while (boolMarks[intCurrentWord])
                        {
                            if (textBox2.Text == "" || textBox3.Text == "")
                                intCurrentWord = randomWord.Next(stringSEn.Length);
                            else
                                intCurrentWord = randomWord.Next(intR1, intR2);
                            int1++;
                            if (int1 == 100)
                                return;
                        }
                    }
                }
                else
                    intCurrentWord++;
                liBoEn.SelectedIndex = intCurrentWord;
            }
            else if (e.KeyCode == Keys.Q||e.KeyCode==Keys.Up)
            {
                if (textBoxCn.Text == "")
                {
                    textBoxCn.Text = stringSCn[intCurrentWord];
                    return;
                }
                if (boolPrecluding)
                    do
                    {
                        if (intCurrentWord == 0)
                            intCurrentWord = intTally2 + 1;
                        intCurrentWord--;
                    }
                    while (boolMarks[intCurrentWord]);
                else if (intCurrentWord == 0)
                    intCurrentWord = intTally2;
                else
                    intCurrentWord--;
                liBoEn.SelectedIndex = intCurrentWord;
            }
            else if (e.KeyCode == Keys.E)
            {
                boolMarks[intCurrentWord] = !boolMarks[intCurrentWord];
                if (boolMarks[intCurrentWord])
                {
                    labelEn.ForeColor = System.Drawing.Color.Blue;
                    liBoCn.Items[intCurrentWord] = stringSCn[intCurrentWord];
                }
                else
                {
                    labelEn.ForeColor = System.Drawing.Color.White;
                    liBoCn.Items[intCurrentWord] = "";
                }
            }
            else if (e.KeyCode == Keys.X)
            {
                boolPrecluding = !boolPrecluding;
                label2.Text = "Precluding " + boolPrecluding.ToString();
            }
            else if (e.KeyCode == Keys.Z)
            {
                boolScrambled = !boolScrambled;
                label1.Text = "Scrambling " + boolScrambled.ToString();
                textBox2.ReadOnly = !textBox2.ReadOnly;
                textBox3.ReadOnly = !textBox3.ReadOnly;
                if (textBox2.Text == "" || textBox3.Text == "")
                    return;
                if (boolScrambled)
                {
                    intR1 = Convert.ToInt32(textBox2.Text);
                    intR2 = Convert.ToInt32(textBox3.Text);
                }
            }
            else if (e.KeyCode == Keys.C)
                for (int i = 0; i != stringSEn.Length; i++)
                    stringSEn[i] = stringSEn[i].ToUpper();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            diReClosing = MessageBox.Show("Save it?", "WordMemorizingApp", MessageBoxButtons.YesNo);
            if (diReClosing == DialogResult.Yes)
            {
                StreamWriter streamWriter1 = new StreamWriter("marks.txt");
                bool b=false;
                int i = 0;
                string t="";
                foreach (bool s in boolMarks)
                {
                    if (s == b)
                        i++;
                    else
                    {
                        t += ' ' + i.ToString();
                        i = 1;
                        b = !b;
                    }
                }
                if (b)
                    t += ' ' + i.ToString();
                t = t.Substring(1);
                streamWriter1.Write(t);
                streamWriter1.Close();
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            foreach (char c in textBox1.Text)
                if (c < '0' || c > '9')
                    return;
            int i = Convert.ToInt32(textBox1.Text);
            if (i > intTally2)
                return;
            else if (boolMarks[i] && boolPrecluding)
                return;
            intCurrentWord = i;
            liBoEn.SelectedIndex = intCurrentWord;
        }

        private void liBoEn_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (liBoEn.SelectedItem == null)
                return;
            if (boolMarks[liBoEn.SelectedIndex])
            {
                if (boolPrecluding)
                {
                    int int1=liBoEn.SelectedIndex;
                    do
                    {
                        int1++;
                        if (int1 == intTally2)
                            int1 = 0;
                    }
                    while (boolMarks[int1]);
                    liBoEn.SelectedIndex = int1;
                    labelEn.ForeColor = System.Drawing.Color.White;
                }
                else
                    labelEn.ForeColor = System.Drawing.Color.Blue;
            }
            else
                labelEn.ForeColor = System.Drawing.Color.White;
            labelEn.Text = stringSEn[liBoEn.SelectedIndex];
            label3.Text = liBoEn.SelectedIndex.ToString();
            Clipboard.SetText(labelEn.Text);
            textBox4.Clear();
            stringOTWord = labelEn.Text.Remove(labelEn.Text.IndexOf(' '));
            foreach (string z in stringSClusters)
            {
                foreach (string x in z.Split(' '))
                {
                    if (x.ToLower() == stringOTWord.ToLower())
                    {
                        textBox4.AppendText(' ' + z + '\n' + '\n');
                        break;
                    }
                }
            }
            textBoxCn.Text = "";
            intCurrentWord = liBoEn.SelectedIndex;
            liBoCn.SelectedIndex = liBoEn.SelectedIndex;
            liBoEn.TopIndex = intCurrentWord - 20;
            liBoCn.TopIndex = liBoEn.TopIndex;
            textBoxCn.Focus();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            streamReader1 = new StreamReader("wordclusters.txt");
            stringClusters = streamReader1.ReadToEnd();
            streamReader1.Close();
            stringSClusters = stringClusters.Split(splittingStrings, StringSplitOptions.RemoveEmptyEntries);
        }

        private void liBoCn_SelectedIndexChanged(object sender, EventArgs e)
        {
            textBoxCn.Focus();
        }
    }
}
