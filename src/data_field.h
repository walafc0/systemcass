/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 data_field.h                      |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   03_05_2006                      |
|                                                             |
\------------------------------------------------------------*/
#ifndef __DATA_FIELD_H__
#define __DATA_FIELD_H__

template <int WIDTH, int PADDING, typename data_type>
struct val_field {
  data_type valW:WIDTH;
};

template <int WIDTH, typename data_type>
struct val_field<WIDTH, 0, data_type> {
  data_type valW:WIDTH;
};

#endif

