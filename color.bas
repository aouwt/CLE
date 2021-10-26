$NOPREFIX
$RESIZE:STRETCH
'$COLOR:32
'$INCLUDE:'../qb64/internal/support/color/color32_noprefix.bi'
'$DYNAMIC
'OPTION EXPLICIT
TYPE Colors
    AS UNSIGNED LONG L, R, U, D
END TYPE

TYPE Cell
    AS STRING * 1 Thing
    AS Colors C
END TYPE



DIM SHARED BlankCell AS Cell
DIM SHARED Board(512, 512) AS Cell
DIM SHARED AS UNSIGNED INTEGER CellW, CellH
LoadBoard "light.txt"

f& = 8
CellW = FONTWIDTH(f&): CellH = FONTHEIGHT(f&)
SCREEN NEWIMAGE(UBOUND(Board, 1) * CellW, UBOUND(Board, 2) * CellH, 32)
FONT f&
PRINTMODE KEEPBACKGROUND

DIM x~%, y~%
DO
    UpdateBeams
    FOR x~% = 0 TO UBOUND(Board, 1): FOR y~% = 0 TO UBOUND(Board, 2)
            CALL CheckCell(Board(x~%, y~%))
    NEXT y~%, x~%
    'DrawBoard: ' SLEEp
    DrawBoard ': SLEEP
LOOP


SUB LoadBoard (filename$)
    DIM fn~%, y~%, ln$, w~%, x~%, h~%

    DIM NewBoard(UBOUND(Board, 1), UBOUND(Board, 2)) AS Cell

    fn~% = FREEFILE
    OPEN filename$ FOR INPUT AS #fn~%

    DO
        LINE INPUT #fn~%, ln$
        IF w~% < LEN(ln$) THEN w~% = LEN(ln$)

        FOR x~% = 1 TO LEN(ln$)
            NewBoard(x~% - 1, h~%).Thing = CHR$(ASC(ln$, x~%))
        NEXT
        h~% = h~% + 1
    LOOP UNTIL EOF(fn~%)

    REDIM Board(w~%, h~%) AS Cell

    FOR x~% = 0 TO w~%: FOR y~% = 0 TO h~%
            Board(x~%, y~%) = NewBoard(x~%, y~%)
    NEXT y~%, x~%

    CLOSE #fn~%
END SUB



SUB UpdateBeams
    DIM NewBoard(UBOUND(Board, 1), UBOUND(Board, 2)) AS Colors
    'DIM newboard(16, 16) AS Colors
    DIM x~%, y~%
    FOR x~% = 0 TO UBOUND(Board, 1): FOR y~% = 0 TO UBOUND(Board, 2)
            NewBoard(x~%, y~%) = Board(x~%, y~%).C
            Board(x~%, y~%).C = BlankCell.C
    NEXT y~%, x~%
    FOR x~% = 0 TO UBOUND(Board, 1)
        FOR y~% = 0 TO UBOUND(Board, 2)

            IF x~% <> UBOUND(Board, 1) THEN Board(x~%, y~%).C.L = NewBoard(x~% + 1, y~%).L
            IF y~% <> UBOUND(Board, 2) THEN Board(x~%, y~%).C.U = NewBoard(x~%, y~% + 1).U
            IF x~% THEN Board(x~%, y~%).C.R = NewBoard(x~% - 1, y~%).R
            IF y~% THEN Board(x~%, y~%).C.D = NewBoard(x~%, y~% - 1).D

    NEXT y~%, x~%
END SUB


SUB DrawBoard
    CLS
    _LIMIT 10
    DIM x~%, y~%, px~%, py~%
    FOR x~% = 0 TO UBOUND(Board, 1)
        px~% = x~% * CellW
        FOR y~% = 0 TO UBOUND(Board, 2)
            py~% = y~% * CellH
            IF Board(x~%, y~%).C.L THEN LINE (px~%, py~% + CellH / 2)-STEP(CellW, 0), Board(x~%, y~%).C.L
            IF Board(x~%, y~%).C.R THEN LINE (px~%, py~% + CellH / 2)-STEP(CellW, 0), Board(x~%, y~%).C.R
            IF Board(x~%, y~%).C.U THEN LINE (px~% + CellW / 2, py~%)-STEP(0, CellH), Board(x~%, y~%).C.U
            IF Board(x~%, y~%).C.D THEN LINE (px~% + CellW / 2, py~%)-STEP(0, CellH), Board(x~%, y~%).C.D


            'IF Board(x~%, y~%).Thing = " " THEN
            '    PRINTSTRING (px~%, py~%), "ù"
            ' ELSE
            PRINTSTRING (px~%, py~%), Board(x~%, y~%).Thing
            'END IF
    NEXT y~%, x~%
    _DISPLAY
END SUB



SUB CheckCell (Cell AS Cell)
    DIM NewC AS Colors


    SELECT CASE Cell.Thing
        'COLORS!
        CASE "R": CALL DistColor(Cell.C, C_Red)
        CASE "G": CALL DistColor(Cell.C, C_Green)
        CASE "B": CALL DistColor(Cell.C, C_Blue)
        CASE "W": CALL DistColor(Cell.C, C_White)
            'CASE "D": CALL DistColor(Cell.C, C_Black)
        CASE "Y": CALL DistColor(Cell.C, C_Yellow)
        CASE "C": CALL DistColor(Cell.C, C_Cyan)
        CASE "M": CALL DistColor(Cell.C, C_Magenta)

        CASE "r": CALL DistColor(Cell.C, C_Red): Cell.Thing = ""
        CASE "g": CALL DistColor(Cell.C, C_Green): Cell.Thing = ""
        CASE "b": CALL DistColor(Cell.C, C_Blue): Cell.Thing = ""
        CASE "w": CALL DistColor(Cell.C, C_White): Cell.Thing = ""
            'CASE "d": CALL DistColor(Cell.C, C_Black): Cell.Thing = ""
        CASE "y": CALL DistColor(Cell.C, C_Yellow): Cell.Thing = ""
        CASE "c": CALL DistColor(Cell.C, C_Cyan): Cell.Thing = ""
        CASE "m": CALL DistColor(Cell.C, C_Magenta): Cell.Thing = ""


            'LIGHT MANIPULATION
        CASE "#" 'Shade
            Cell.C.L = RGB32(RED32(Cell.C.L) / 2, GREEN32(Cell.C.L) / 2, BLUE32(Cell.C.L) / 2)
            Cell.C.R = RGB32(RED32(Cell.C.R) / 2, GREEN32(Cell.C.R) / 2, BLUE32(Cell.C.R) / 2)
            Cell.C.U = RGB32(RED32(Cell.C.U) / 2, GREEN32(Cell.C.U) / 2, BLUE32(Cell.C.U) / 2)
            Cell.C.D = RGB32(RED32(Cell.C.D) / 2, GREEN32(Cell.C.D) / 2, BLUE32(Cell.C.D) / 2)


        CASE "@" 'Diffuser: Distribute average color
                CALL DistColor(Cell.C, RGB32( _
                    (RED32(Cell.C.L) + RED32(Cell.C.R) + RED32(Cell.C.U) + RED32(Cell.C.D)) / 4, _
                    (GREEN32(Cell.C.L) + GREEN32(Cell.C.R) + GREEN32(Cell.C.U) + GREEN32(Cell.C.D)) / 4, _
                    (BLUE32(Cell.C.L) + BLUE32(Cell.C.R) + BLUE32(Cell.C.U) + BLUE32(Cell.C.D)) / 4) _
                )

        CASE "{" 'solar panel
            w~& = (RED32(Cell.C.L) + GREEN32(Cell.C.L) + BLUE32(Cell.C.L)) / 3
            Cell.C.L = RGB32(w~&, w~&, w~&)
            Cell.C.U = 0: Cell.C.D = 0: Cell.C.R = 0

            'LIGHT FLOW
        CASE "<": Cell.C.L = Merge(Cell.C)
        CASE ">": Cell.C.R = Merge(Cell.C)
        CASE "^": Cell.C.U = Merge(Cell.C)
        CASE "v": Cell.C.D = Merge(Cell.C)

        CASE "(": IF Cell.C.L THEN Cell.C.R = Cell.C.L: Cell.C.L = 0
        CASE ")": IF Cell.C.R THEN Cell.C.L = Cell.C.R: Cell.C.R = 0

            'case "[":if


        CASE "\"
            NewC.D = Cell.C.R 'Left reflect down
            NewC.U = Cell.C.L 'right reflect up
            NewC.L = Cell.C.U 'Up reflect left
            NewC.R = Cell.C.D 'Down reflect right
            Cell.C = NewC
        CASE "/"
            NewC.U = Cell.C.R 'Left reflect up
            NewC.D = Cell.C.L 'right reflect down
            NewC.R = Cell.C.U 'Up reflect right
            NewC.L = Cell.C.D 'Down reflect left
            Cell.C = NewC

        CASE "|": Cell.C.L = 0: Cell.C.R = 0
        CASE "-": Cell.C.U = 0: Cell.C.D = 0

            'CASE "#"
            '    tmp~%% = (Cell.C.L <> 0) + (Cell.C.R <> 0) + (Cell.C.U <> 0) + (Cell.C.D <> 0)
            '    IF tmp~%% < 2 THEN Cell.C = BlankCell.C

        CASE " "
            DIM AS UNSIGNED LONG VC, HC
            IF (Cell.C.L <> 0) AND (Cell.C.R <> 0) THEN
                HC = RGB32(RED32(Cell.C.L) + RED32(Cell.C.R), GREEN32(Cell.C.L) + GREEN32(Cell.C.R), BLUE32(Cell.C.L) + BLUE32(Cell.C.R))
                Cell.C.L = HC
                Cell.C.R = HC
            ELSE HC = Cell.C.R OR Cell.C.L
            END IF

            IF (Cell.C.U <> 0) AND (Cell.C.D <> 0) THEN
                VC = RGB32(RED32(Cell.C.U) + RED32(Cell.C.D), GREEN32(Cell.C.U) + GREEN32(Cell.C.D), BLUE32(Cell.C.U) + BLUE32(Cell.C.D))
                Cell.C.U = VC
                Cell.C.D = VC
            ELSE VC = Cell.C.U OR Cell.C.D
            END IF

            IF HC <> 0 AND VC <> 0 THEN 'intersection filter
                Cell.C.L = Filter(Cell.C.L, VC)
                Cell.C.R = Filter(Cell.C.R, VC)
                Cell.C.U = Filter(Cell.C.U, HC)
                Cell.C.D = Filter(Cell.C.D, HC)
            END IF

        CASE ELSE: Cell.Thing = " "
    END SELECT

END SUB


FUNCTION Merge~& (C AS Colors)
    Merge = RGB32(RED32(C.L) + RED32(C.R) + RED32(C.U) + RED32(C.D), GREEN32(C.L) + GREEN32(C.R) + GREEN32(C.U) + GREEN32(C.D), BLUE32(C.L) + BLUE32(C.R) + BLUE32(C.U) + BLUE32(C.D))
    'C.L = 0: C.R = 0: C.U = 0: C.D = 0
END FUNCTION


FUNCTION Filter~& (IC~&, FC~&)
    Filter~& = RGB32(RED32(IC~&) * (RED32(FC~&) / 255), GREEN32(IC~&) * (GREEN32(FC~&) / 255), BLUE32(IC~&) * (BLUE32(FC~&) / 255))
END FUNCTION

SUB DistColor (C AS Colors, C~&)
    DIM M~&
    M~& = RGB32(_
        RED32(C.L) + RED32(C.R) + RED32(C.U) + RED32(C.D) + RED32(C~&),_
        GREEN32(C.L) + GREEN32(C.R) + GREEN32(C.U) + GREEN32(C.D) + GREEN32(C~&),_
        BLUE32(C.L) + BLUE32(C.R) + BLUE32(C.U) + BLUE32(C.D) + BLUE32(C~&)_
    )
    C.L = M~&: C.R = M~&: C.U = M~&: C.D = M~&
END SUB
