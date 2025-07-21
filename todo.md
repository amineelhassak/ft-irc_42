read_size = recv(poll_fds[i].fd, buff, sizeof(buff), 0);
                    if (read_size <= 0)
                    {
                        close(poll_fds[i].fd);
                        poll_fds.erase(poll_fds.begin() + i);
                        --i;
                        continue;
                    }
don't erase the file discriptor directly from the vector
