#!/bin/python3
"""
 *  NOTE      - gen.py
 *  Author    - Chae Lee-Jin
 *
 *  Created   - 2024.05.31
 *  Github    - https://github.com/aruyu
 *  Contact   - vine9151@gmail.com
"""

import os
#import numpy as np
#import pandas as pd
import argparse
from argparse import RawTextHelpFormatter


class GenerateEnv:
  """ Class of Generate Environment """

  def __init__(self, input):
    self.input = input
    self.state = False

    try:
      self.gen_string = self.make_front() + self.make_back()
      self.state = True
    except:
      print("'{0}': No such file or directory".format(self.input))

  def make_front(self):
    """ Function to Make Front """

    retval = ""
    front = ["#if defined(CONFIG_MICROBLAZE)\n",
             "#include <configs/microblaze-generic.h>\n",
             "#define CONFIG_SYS_BOOTM_LEN 0xF000000\n",
             "#endif\n",
             "#if defined(CONFIG_ARCH_ZYNQ)\n",
             "#include <configs/zynq-common.h>\n",
             "#endif\n",
             "#if defined(CONFIG_ARCH_ZYNQMP)\n",
             "#include <configs/xilinx_zynqmp.h>\n",
             "#endif\n",
             "#if defined(CONFIG_ARCH_VERSAL)\n",
             "#include <configs/xilinx_versal.h>\n",
             "#endif\n",
             "#if defined(CONFIG_ARCH_VERSAL_NET)\n",
             "#include <configs/xilinx_versal_net.h>\n",
             "#endif\n\n"]

    for i in range(len(front)):
      retval += front[i]

    return retval

  def make_back(self):
    """ Function to Make Back """

    retval = ""
    back = ["\n#if 1\n\n",
            "#undef CONFIG_EXTRA_ENV_SETTINGS\n",
            "#define CONFIG_EXTRA_ENV_SETTINGS \\\n",
            "    ENV_MEM_LAYOUT_SETTINGS \\\n",
            "    BOOTENV \\\n"]

    lf = "\\\n"

    input_file = open(self.input, 'r')

    for line in input_file.readlines():
      if line.startswith("setenv "):
        tmp = line.split()
        back.append("    \"" + tmp[1] + '=')
        cnt = len(tmp)

        for index, value in enumerate(tmp):
          if index == 2:
            if value.startswith('\"') or value.startswith('\''):
              back.append(value[1:] + ' ')
            else:
              if cnt != 3:
                back.append(value + ' ')
              else:
                back.append(value)

          elif index > 2 and index < cnt-1:
            back.append(value + ' ')

          elif index == cnt-1:
            if ('\"' in value) or ('\'' in value):
              back.append(value[:-1])
            else:
              back.append(value)

        back.append("\\0\" " + lf)

      elif line.startswith("\n") or line.startswith("\r"):
        back.append("    " + lf)

    back.append("\n#endif")
    input_file.close()

    for i in range(len(back)):
      retval += back[i]

    return retval

  def save_file(self, output):
    """ Function to Save File """

    if self.state:
      try:
        output_file = open(output, 'w')
      except:
        print("'{0}': No such file or directory".format(output))
      else:
        output_file.write(self.gen_string)
        output_file.close()


if __name__ == "__main__":
  parser = argparse.ArgumentParser(description="Generate bootenv to 'platform-top.h'\n" +
                                               "MIT License <https://github.com/aruyu/petalinux-configs/blob/master/LICENSE/>\n" +
                                               "Report or pull request any time. <https://github.com/aruyu/petalinux-configs/>",
                                   formatter_class=RawTextHelpFormatter)
  parser.add_argument('-i', '--input', required=True,
                      help="Input file name that want to generate.\n" +
                           "Ex) ./bootenv")
  parser.add_argument('-o', '--output', required=False, default="./platform-top.h",
                      help="Output file name that want to be generated.\n" +
                           "Ex) ./platform-top.h")
  args = parser.parse_args()

  gen = GenerateEnv(args.input)
  gen.save_file(args.output)
