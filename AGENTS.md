# AGENTS.md — Orchid

## Projekt und Verzeichnisse

Orchid ist ein Eden/yuzu-basierter Switch-2-Emulator (C++20, CMake/Ninja, Catch2).
Dieses Verzeichnis ist ein Quellcode-Checkout. Lokale Testartefakte bleiben unter
`/home/leon/Orchid/`; aus Worktrees immer deren absolute Pfade verwenden.

- `orchid-builds/<label>/`: archivierte Orchid-/Lichii-Binaries für Vergleiche.
- `orchid-perf/<datum>_<zeit>-<label>/`: Messläufe mit `meta.txt`, `summary.txt`,
  MangoHud-CSV, `threads.log`, `perf.data`, `perf-top.txt`, `eden_log.txt` und ggf. `qt-config.ini`.
- `orchid-issues/`: nach Issue nummerierte Untersuchungen, Logs und übergreifende Build-Logs.
- Quellcode: `/home/leon/src/Eglantine`; weitere Worktrees unter `/home/leon/src/Eglantine-*`
  sowie ggf. anderen Pfaden. Ordnernamen tragen noch den früheren Projektnamen.
- Vor Code-Arbeit `git -C /home/leon/src/Eglantine worktree list` ausführen und den
  beauftragten Worktree bestimmen. Rollen und Branches nicht aus Ordnernamen ableiten.
- Im gewählten Checkout dessen `AGENTS.md`/`AGENTS.override.md` lesen.
  `/home/leon/src/Eglantine/AGENTS.override.md` enthält bestehende Build-, Code- und Testregeln.
  Diese Datei hier ersetzt sie nicht.
- Spieldumps unter `/home/leon/Switch2/` nur lesen. Bestehende Messungen und archivierte
  Binaries erhalten; neue Ergebnisse mit eindeutigem Label ablegen.

## Zielspiele

Entwicklungs- und Supportfokus gemäß [Project scope der GitHub-README](https://github.com/MythosOfs/Orchid/blob/main/README.md#project-scope)
(Stand: 2026-11-06), ergänzt um Orbitals auf Leons Auftrag:

- Mario Tennis Fever
- Beide Hyrule-Warriors-Spiele (Bezeichnung aus der README)
- Pokémon Pokopia
- Star Fox
- Super Mario Bros. Wonder
- Yoshi and the Mysterious Book
- Donkey Kong Bananza
- Kirby Air Riders
- Kirby and the Forgotten Land
- Orbitals

Diese Liste beschreibt die Zielspiele, nicht zugesicherte Spielbarkeit. Die separate
Kompatibilitätstabelle der README enthält weitere getestete Titel und aktuelle Befunde.

## Build und Prüfung

Im gewählten Checkout arbeiten. Konfiguration und vollständige CMake-Flags stehen in
`/home/leon/src/Eglantine/AGENTS.override.md`; vorhandene Build-Konfiguration zuerst prüfen.
Für einen bereits konfigurierten `build`-Ordner:

```sh
nice ninja -C build -j6 orchid
nice ninja -C build -j6 tests
build/bin/tests "<passender Catch2-Filter>"
```

- GUI-Binary: `build/bin/orchid`. Den konkreten Build testen und Pfad/Commit dokumentieren.
- Vor schweren Läufen auf laufende Builds prüfen (`pgrep -x cc1plus`, `pgrep -x ninja`)
  und vorhandene Jobs abwarten. Builds über die vorhandenen `ninja`-/`cmake`-Wrapper,
  andere schwere Agent-Jobs über `agent-run` starten (eigene systemd-Slice).
- Lokales `RelWithDebInfo` nutzt laut Projektregeln `ENABLE_WERROR=OFF`; CI-Warnungen
  und andere Architekturen zusätzlich berücksichtigen.
- Headless-Regressionstests: `/home/leon/.hermes/cache/scratch/sw2test/WORKER.md` lesen;
  eigener freier Slot, isolierte Konfiguration/Daten und explizites `BIN` sind erforderlich.
  Den dort und im Checkout beschriebenen Mario-Tennis-Fever-Regressionscheck beachten.
- Änderungen mit passenden Tests prüfen. Visuelle Fehler auch anhand von Screenshots
  bewerten; Exitstatus allein belegt keinen erfolgreichen Spielstart.

## Starten und Performance messen

Die vorhandenen Helfer liegen in `/home/leon/.local/bin/`:

```sh
orchid-test-run <eindeutiges-label> /absoluter/pfad/zum/orchid [emulator-argumente...]
orchid-perf-summary /absoluter/pfad/zum/messlauf
```

- `orchid-test-run` zeichnet MangoHud, Threadlast, CPU-Profil und Emulatorlog auf und
  friert währenddessen `agent-work.slice` ein. Der Lauf verwendet die normale Eden-
  Konfiguration; er ist kein isolierter Headless-Test und öffnet die GUI auf dem Host.
- Achtung: Der Helfer schreibt aktuell nach `/home/leon/orchid-perf/`. Die vorhandenen
  Messungen liegen dagegen hier unter `/home/leon/Orchid/orchid-perf/`; den tatsächlich
  ausgegebenen Ergebnispfad verwenden und neue Läufe anschließend hier einordnen.
- `orchid-latest` wählt nach Änderungszeit automatisch das neueste gefundene Binary.
  Für reproduzierbare Vergleiche einen expliziten Binary-Pfad verwenden.
- Für A/B-Vergleiche Spielszene, Einstellungen, Auflösung, Powerprofil und Cache-Zustand
  vergleichbar halten; Build/Commit und Messdauer angeben. Keine konkurrierenden Lasttests.
- `summary.txt` dient als Einstieg; dessen Engpassdiagnose ist eine Heuristik.
  Auffälligkeiten mit CSV, `perf-top.txt` und `eden_log.txt` belegen.
- Normale Emulatorlogs: `/home/leon/.local/share/eden/log/eden_log.txt`.
  Auf `Device loss`, abgelehnte Shader, `unimplemented function` und Abstürze achten.

## Sicherheit und Freigaben

- Secrets, Tokens, Schlüssel und private Nummern nie im Klartext ausgeben oder einchecken.
- Fragen untersuchen und beantworten; Änderungen nur bei eindeutigem Arbeitsauftrag.
- Beauftragte reversible Arbeit selbstständig erledigen und prüfen; keine Bestätigungen
  für routinemäßige Zwischenschritte einholen.
- Vor destruktiven oder verbindungsunterbrechenden Aktionen warnen. Pushes, Releases,
  Veröffentlichungen und Nachrichten an andere nur mit ausdrücklichem Auftrag.
  Eine erteilte Freigabe gilt für denselben Umfang weiter, auch für Subagents.
- Nur selbst gestartete Emulatorprozesse beenden. Leons Desktop und echte Konfiguration
  schützen: isolierte Headless-Tests bevorzugen; Host-GUI für notwendige GPU-/Spieletests.

## Zusammenarbeit

- Kurz und direkt auf Deutsch berichten, üblicherweise 1–2 Sätze; nötige Befunde,
  Risiken und Prüfergebnisse trotzdem vollständig nennen.
- Messungen, Logs und Primärquellen bevorzugen. Keine erfundenen Ergebnisse,
  ungetesteten Erfolgsmeldungen oder Stubs als fertige Lösung.
- Sinnvoll unabhängige größere Aufgaben delegieren, maximal drei Subagents parallel.
  Kleine oder strikt sequenzielle Aufgaben selbst erledigen.
- Keine Sleep-Schleifen zum Warten; Prozessende, Ereignisse oder die verfügbaren
  ereignisgesteuerten Wartewerkzeuge nutzen.

## Verbindliche Auslieferung eines Testbuilds

- Nach Emulator-Codeänderungen erhält Leon am Ende einen tatsächlich gebauten Testbuild.
- Für diesen konkreten Build eine `.desktop`-Datei unter
  `~/.local/share/applications/` installieren, damit er in Leons App-Launcher verfügbar ist.
- Den Eintrag eindeutig als Testbuild mit Datum/Stand benennen. `Exec` muss den
  ausgelieferten Build oder dessen Starter aufrufen; das Binary dauerhaft aufbewahren.
- Ein separates Testprofil verwenden, Leons normale Konfiguration schützen und den
  Starter prüfen. Im Abschluss Namen des Launcher-Eintrags, Binary-Pfad, Commit,
  Tests und bekannte Einschränkungen nennen. Ein Shellskript allein genügt nicht.
