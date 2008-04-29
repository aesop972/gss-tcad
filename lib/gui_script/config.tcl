#--- configure for default fonts

foreach type {normal italic bold fixed large} {
  set Font($type) ""
}

#----- init fonts and colors

proc do_config {args} {
  global argc argv bgColor tkPalette tk_strictMotif UseNativeDialogs
  set config ""
  set proc ""
  set fg ""
  set bg ""
  set native 0
  set fonts 0
  set motif 0
  set focus 0
  if {$argc} {
    eval lappend args $argv
  }
  set len [llength $args]
  if {$len} {
    for {set n 0} {$n < $len} {incr n} {
      set arg [string tolower [lindex $args $n]]
      if [string match "-con*" $arg] {
        incr n
        if {$n < $len} {
          set config [lindex $args $n]
        }
      } elseif [string match "-win*" $arg] {
        set proc setup_windows
        set config ""
      } elseif [string match "-def*" $arg] {
        set proc config_init
        set config ""
      } elseif [string match "-fon*" $arg] {
        set fonts 1
      } elseif [string match "-nat*" $arg] {
        set native 1
      } elseif [string match "-fgc*" $arg] {
        incr n
        if {$n < $len} {
          set fg [lindex $args $n]
        }
      } elseif [string match "-bgc*" $arg] {
        incr n
        if {$n < $len} {
          set bg [lindex $args $n]
        }
      } elseif [string match "-mot*" $arg] {
        set motif 1
      } elseif [string match "-foc*" $arg] {
        set focus 1
      }
    }
  }

  if {$config != "" && ![catch {option readfile $config}]} {
    set UseNativeDialogs 0
    config_init
    if {$fonts} setup_fonts
    . configure -background $bgColor(normal)
    if {[option get . keyboardFocusPolicy TakeFocus] == "pointer"} {
      set focus 1
    }
  } elseif {$proc != ""} {
    $proc
  } else {
    if {$bg != ""} {
      if {$fg == ""} {
        catch {tk_setPalette $bg}
      } else {
        catch {tk_setPalette background $bg foreground $fg}
      }
      option add *background $tkPalette(background)
      option add *foreground $tkPalette(foreground)
      . configure -background $tkPalette(background)
    }
    if {$fonts} setup_fonts
    config_init
  }
  if {$native} {set UseNativeDialogs 1}
  if {$motif} {
    set tk_strictMotif 1
    set focus 0
  }
  if {$focus} {
    if {[info commands tk_focusFollowsMouse] != ""} {
      tk_focusFollowsMouse
    } else {
      bind all <Enter> {+focus %W}
    }
  }
}

proc setup_fonts {} {
  global Font tcl_platform
  if {$tcl_platform(platform) == "windows"} {
    set Font(normal) "Helvetica 8"
    set Font(italic) "Helvetica 8 italic"
    set Font(bold)   "Helvetica 8 bold"
    set Font(fixed)  "Courier 8"
    set Font(large)  "Helvetica 10"
  } else {
    set Font(normal) "Helvetica -12"
    set Font(italic) "Helvetica -12 italic"
    set Font(bold)   "Helvetica -12 bold"
    set Font(fixed)  "Courier -12"
    set Font(large)  "Helvetica -16"
  }
  option add *font $Font(normal)
  option add *Entry.font $Font(fixed)
  option add *Listbox.font $Font(fixed)
  option add *Text.font $Font(fixed)
}

#--- setup colors for windows

proc setup_windows {} {
  global UseNativeDialogs tcl_platform
  if {$tcl_platform(platform) == "windows"} {
    set UseNativeDialogs 1
    option add *Listbox.background SystemWindow
    option add *Listbox.foreground SystemWindowText
#    option add *Canvas.background SystemWindow
    option add *Button.padY 0
  } else {
    set UseNativeDialogs 0
    tk_setPalette #C0C0C0
    option add *selectBackground #00007F
    option add *selectForeground white
    option add *Text.background white
    option add *Entry.background white
    option add *Listbox.background white
#    option add *Canvas.background white
  }
  setup_fonts
  config_init
}

#--- get colors and fonts into globals

proc config_init {} {
  global Font bgColor fgColor tcl_platform UseNativeDialogs

  checkbutton .configbutton
  text .configtext
  label .configlabel

  #--- background colors

  set bgColor(normal)    [.configlabel cget -background]
  set bgColor(button)    [.configbutton cget -background]
  set bgColor(text)      [.configtext cget -background]
  set bgColor(active)    [.configbutton cget -activebackground]
  set bgColor(highlight) [.configbutton cget -highlightbackground]
  set bgColor(disabled)  $bgColor(normal)
  set bgColor(select)    [.configtext cget -selectbackground]
  set bgColor(indicator) [.configbutton cget -selectcolor]
  if {![info exists bgColor(canvas)]} {
    canvas .configcanvas
    set bgColor(canvas) [.configcanvas cget -background]
    destroy .configcanvas
  }

  #--- foreground colors

  set fgColor(normal)    [.configlabel cget -foreground]
  set fgColor(button)    [.configbutton cget -foreground]
  set fgColor(text)      [.configtext cget -foreground]
  set fgColor(active)    [.configbutton cget -activeforeground]
  set fgColor(highlight) [.configbutton cget -highlightcolor]
  set fgColor(disabled)  [.configbutton cget -disabledforeground]
  set fgColor(select)    [.configtext cget -selectforeground]
  set fgColor(indicator) $bgColor(indicator)
  if {![info exists fgColor(canvas)]} {
    set mx [lindex [winfo rgb . white] 0]
    set bg [winfo rgb . $bgColor(canvas)]
    if [expr (0.3 * [lindex $bg 0] + 0.59 * [lindex $bg 1] + \
        0.11 * [lindex $bg 2]) >= (0.5 * $mx)] {
      set fgColor(canvas) black
    } else {
      set fgColor(canvas) white
    }
  }

  #--- fonts

  foreach type {normal fixed italic bold large} {
    if {$Font($type) == ""} {
      set font [option get . $type\Font Font]
      if {$font != ""} {
        if ![catch {button .fntbutton -font $font}] {
          set Font($type) $font
        }
        catch {destroy .fntbutton}
      }
    }
  }

  set font [.configlabel cget -font]
  set family [lindex $font 0]
  set size [lindex $font 1]
  if {$Font(normal) == ""} {
    set Font(normal) [list $family $size]
  }
  if {$Font(fixed) == ""} {
    set Font(fixed) [list Courier $size]
  }
  if {$Font(bold) == ""} {
    set Font(bold) [list $family $size bold]
  }
  if {$Font(italic) == ""} {
    set Font(italic) [list $family $size italic]
  }
  if {$Font(large) == ""} {
    if {$size < 0} {
      incr size -2
    } else {
      incr size 2
    }
    set Font(large) [list $family $size]
  }

  destroy .configbutton
  destroy .configtext
  destroy .configlabel

  if {![info exists UseNativeDialogs] || $UseNativeDialogs == ""} {
    if {$tcl_platform(platform) == "windows"} {
      set UseNativeDialogs 1
    } else {
      set UseNativeDialogs 0
    }
  }
}
