﻿using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent(); 
            allDrives = DriveInfo.GetDrives();
            foreach (var disk in allDrives)
            {
                if (disk.Name != "C:\\")
                {
                    listBox1.Items.Add(disk.Name);
                }
            }
            foreach (var disk in allDrives)
            {
                if (disk.Name != "C:\\")
                {
                    DirectoryInfo di = new DirectoryInfo(disk.Name);
                    foreach (var papka in di.GetDirectories().Where(x => (x.Attributes & FileAttributes.Hidden) == 0))
                    {
                        folders.Add(papka.Name);
                    }
                }
            }

            foreach (var info in folders)
            {
                checkedListBox1.Items.Add(info);
            }
        }
        private void copyBatton_Click(object sender, EventArgs e)
        {
            label4.Text = "Proces start";
            var fileNum = fileCounter();
            progressBar1.Visible = true;
            progressBar1.Minimum = 1;
            progressBar1.Maximum = fileNum;
            progressBar1.Value = 1;
            progressBar1.Step = 1;
            foreach (var folderName in checkedListBox1.CheckedItems)
            {
                var pathWhere = filetext +"\\"+ folderName+"\\";
                Directory.CreateDirectory(pathWhere);
                foreach (var disk in allDrives)
                {
                    if (disk.Name == @"C:\") continue;
                    var curentpath = disk + "" + folderName+"\\";
                    CopyDir(curentpath,pathWhere);
                }
            }

            if (progressBar1.Value == progressBar1.Maximum)
            {
                label4.Text = "Progres finished";
            }

        }
        
        private void checkedListBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (folderBrowserDialog1.ShowDialog() != DialogResult.OK) return;
            filetext = folderBrowserDialog1.SelectedPath;
            textBox1.Text = filetext;
        }
        
        private  void CopyDir(string SourcePath,string DestinationPath)
        {
            foreach (string dirPath in Directory.GetDirectories(SourcePath, "*", 
                SearchOption.AllDirectories))
            {
                try
                {
                    Directory.CreateDirectory(dirPath.Replace(SourcePath, DestinationPath));
                }
                catch(Exception e)
                {
                }
            }
            foreach (string newPath in Directory.GetFiles(SourcePath, "*.*", 
                SearchOption.AllDirectories))
            {
                try
                {
                    File.Copy(newPath, newPath.Replace(SourcePath, DestinationPath), true);
                    progressBar1.PerformStep();
                }
                catch(Exception e)
                {
                }
            }
        }
        
        private int fileCounter()
        {
            int fileCount = 0;
            foreach (var folderName in checkedListBox1.CheckedItems)
            {
                foreach (var disk in allDrives)
                {
                    if (disk.Name == @"C:\") continue;
                    var curentpath = disk + "" + folderName+"\\";
                        
                    try
                    {
                        fileCount += Directory.GetFiles(curentpath, "*", SearchOption.AllDirectories).Length;
                    }
                    catch (Exception exception)
                    { 
                         continue; 
                    }
                }
            }
            return fileCount;
        }

        private string filetext;
        private DriveInfo[] allDrives;
        private HashSet<string> folders= new HashSet<string>();

        private void label4_Click(object sender, EventArgs e)
        {
        }
    }
}