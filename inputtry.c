        char runtime[10];          
        while ((c =fgetc(file1)) != EOF )
        {
           if(!isspace(c))
            {
                if(k==0)   //name of the process is inputted
                {
                    while(!isspace(c))
                    {
                        if(isalpha(c) || isdigit(c))
                        {
                            name_process[i][j]=c;
                            j=j+1;
                            if(j>10)
                            {
                                //error = 1
                                //store the error name using perror
                                //exit(-1)
                            }
                        }
                        else
                        {   
                            //printf error
                            break;
                        }
                        c = fgetc(file1);
                    }//if space, out of the while loop, reset j and add 1 to k, then go to the end and get c for next 

                    j=0;
                    k=k+1;
                }

                else if(k==1) 
                {
                    while(!isspace(c))
                    {
                        if(isdigit(c))
                        {
                            runtime[j]=c;//use string to record runtime
                            j+=1;
                            c = fgetc(file1);
                        }
                        if(isspace(c))
                        {
                            int runt = atoi(runtime);//change string to integer
                            time_process[i]=runt; 
                            j=0; //if space, out of the while loop after this line
                        }
                        if(!isspace(c) ||!isdigit(c) )
                        {
                            //error = 1
                            //store the error name using perror
                            //exit(-1)
                            break;
                        }   
                    }

                    k=k+1;
                }
                else
                {
                    probability_string[i][0]=c;//append first char to stringarray

                    for(int v=1;v<4;v++)//append the rest of three char to arry
                    {
                        c = fgetc(file1);
                        probability_string[i][v]=c;
                    }
                    
                   if (probability_string[i][0]!="0" || probability_string[i][1]!="." || !isdigit(probability_string[i][2]) || !isdigit(probability_string[i][3]))
                   {   
                       break;//check error
                   }
                   
                   if(c==EOF)//check if the end of file
                   {
                       break;
                   }
                   else//if not the end of file, go to next line
                   {
                       c = fgetc(file1);
                   }

                    j=0;
                    k=0;
                    i+=1;
                }
            }
            else
            {
                c = fgetc(file1);//if space , check next char
            }
        } 