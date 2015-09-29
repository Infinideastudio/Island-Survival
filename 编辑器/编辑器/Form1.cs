using System;
using System.IO;
using System.Text;
using System.Windows.Forms;

namespace 编辑器
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        void readData(int i)
        {
            FileStream fs = new FileStream(textBox1.Text + "\\" + i + ".dat", FileMode.Open);
            BinaryReader br = new BinaryReader(fs);
            int id = br.ReadInt32();
            if (id != i)
            {
                MessageBox.Show("错误：文件读取失败，读取的ID号码为" + id + "，应该是" + i + "文件是不是被改名了？");
                br.Close();
                fs.Close();
            }
            label2.Text = id.ToString();
            int descriptionCharsNum = br.ReadInt32();
            string description = Encoding.Default.GetString(br.ReadBytes(descriptionCharsNum));
            textBox2.Text = description;
            int optionscharsNum = br.ReadInt32();
            string options = Encoding.Default.GetString(br.ReadBytes(optionscharsNum));
            textBox3.Text = options;
            br.Close();
            fs.Close();
        }

        void writeData(int i)
        {
            FileStream fs = new FileStream(textBox1.Text + "\\" + i + ".dat", FileMode.Create);
            BinaryWriter bw = new BinaryWriter(fs);
 
            label2.Text = i.ToString();
            bw.Write(i);
            bw.Write((Encoding.Default.GetBytes(textBox2.Text).GetLength(0)));
            bw.Write(Encoding.Default.GetBytes(textBox2.Text));
            bw.Write((Encoding.Default.GetBytes(textBox3.Text).GetLength(0)));
            bw.Write(Encoding.Default.GetBytes(textBox3.Text));
            bw.Flush();
            bw.Close();
            fs.Close();          
        }
        private void button5_Click(object sender, EventArgs e)
        {
            textBox3.SelectedText = "$";
        }
        public int find_the_last()
        {
            int i = 1;
            while(File.Exists(@"" + textBox1.Text +"\\" + i + ".dat"))
            {
                i++;
            }
            return --i;
        }
        private void button4_Click(object sender, EventArgs e)
        {
            textBox3.SelectedText = "{x}>x;yes<no>;";
        }

        private void textBox3_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
            {
                int ss = textBox3.SelectionStart;
                textBox3.Text += "/";
                textBox3.SelectionStart = ss + 1;
            }
        }
        public void read()
        {
            if (File.Exists(textBox1.Text + "\\1.dat")) readData(1);
            else writeData(1);
            max.Text = "Max:" + find_the_last().ToString();
        }
        private void button1_Click(object sender, EventArgs e)
        {
            writeData(int.Parse(label2.Text));
        }

        private void button3_Click(object sender, EventArgs e)
        {
            int i = int.Parse(label2.Text);
            if (i == 1) return;
            writeData(i);
            textBox2.Clear();
            textBox3.Clear();
            i--;
            if (File.Exists(textBox1.Text + "\\" + i + ".dat")) readData(i);
            else writeData(i);
            max.Text = "Max:" + find_the_last().ToString();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            int i = int.Parse(label2.Text);
            writeData(i);
            textBox2.Clear();
            textBox3.Clear();
            i++;
            if (File.Exists(textBox1.Text + "\\" + i + ".dat")) readData(i);
            else writeData(i);
            max.Text = "Max:" + find_the_last().ToString();
        }
        private void select_path_Click(object sender, EventArgs e)
        {
            folderBrowserDialog1.ShowDialog();
            textBox1.Text = folderBrowserDialog1.SelectedPath;
            read();
        }

        private void jump_to_Click(object sender, EventArgs e)
        {
            int i = int.Parse(label2.Text);
            writeData(i);
            textBox2.Clear();
            textBox3.Clear();
            if (File.Exists(textBox1.Text + "\\" + jump_page.Text + ".dat")) readData(int.Parse(jump_page.Text));
            else MessageBox.Show("跳转失败");
        }

        private void delete_Click(object sender, EventArgs e)
        {
            int id = int.Parse(label2.Text);
            if (!(find_the_last() == id) || (id == 1)) return;              
            if (File.Exists(textBox1.Text + "\\" + id+ ".dat"))
            {
                File.Delete(textBox1.Text + "\\" + id + ".dat");         
                int i = int.Parse(label2.Text);
                textBox2.Clear();
                textBox3.Clear();
                i--;
                if (i <= 0) return;
                if (File.Exists(textBox1.Text + "\\" + i + ".dat")) readData(i);
                else writeData(i);
                max.Text = "Max:" + find_the_last().ToString();
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            textBox1.Text = Environment.CurrentDirectory;
            read();
        }
    }
}
