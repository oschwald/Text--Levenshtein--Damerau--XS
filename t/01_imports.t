use strict;
use warnings;

use Test::More tests => 1;

BEGIN { use_ok 'Text::Levenshtein::Damerau::XS', qw/xs_edistance cxs_edistance/ }
